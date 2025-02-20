// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.IO;
using System;
using EpicGames.Core;

public class SML : ModuleRules
{
    public SML(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        bLegacyPublicIncludePaths = false;

        //SML transitive dependencies
        PublicDependencyModuleNames.AddRange(new[] {
            "Json",
            "Projects",
            "NetCore",
            "EnhancedInput",
            "GameplayTags"
        });
        
        PrivateDependencyModuleNames.AddRange(new[] {
            "RenderCore",
            "EngineSettings"
        });
        
        PublicDependencyModuleNames.AddRange(new[] {"FactoryGame"});

        //FactoryGame transitive dependencies
        PublicDependencyModuleNames.AddRange(new[] {
            "Core", "CoreUObject",
            "Engine",
            "InputCore",
            "SlateCore", "Slate", "UMG",
        });

        if (Target.bBuildEditor) {
            PublicDependencyModuleNames.Add("UnrealEd");
            PrivateDependencyModuleNames.Add("MainFrame");
        }

        var ThirdPartyFolder = Path.Combine(ModuleDirectory, "../../ThirdParty");
        PublicIncludePaths.Add(Path.Combine(ThirdPartyFolder, "include"));

        var PlatformName = Target.Platform.ToString();
        var LibraryFolder = Path.Combine(ThirdPartyFolder, PlatformName);

        PublicAdditionalLibraries.Add(Path.Combine(LibraryFolder, "funchook.lib"));
        PublicAdditionalLibraries.Add(Path.Combine(LibraryFolder, "detex.lib"));
        PublicAdditionalLibraries.Add(Path.Combine(LibraryFolder, "libfbxsdk-md.lib"));
        PublicAdditionalLibraries.Add(Path.Combine(LibraryFolder, "libxml2-md.lib"));
        PublicAdditionalLibraries.Add(Path.Combine(LibraryFolder, "zlib-md.lib"));
        PublicAdditionalLibraries.Add(Path.Combine(LibraryFolder, "AssemblyAnalyzer.lib"));
        PublicAdditionalLibraries.Add(Path.Combine(LibraryFolder, "Zydis.lib"));
        PublicAdditionalLibraries.Add(Path.Combine(LibraryFolder, "Zycore.lib"));

        AddFactoryGameInfo();
        AddSMLInfo();
    }

    private void AddFactoryGameInfo()
    {
        const string factoryGameVersionRelativePath = "Source/FactoryGame/currentVersion.txt";

        var FactoryGameVersionFile = FileReference.Combine(Target.ProjectFile!.Directory, factoryGameVersionRelativePath);
        var FactoryGameVersion = File.ReadAllText(FactoryGameVersionFile.FullName);
        PrivateDefinitions.Add($"FACTORYGAME_VERSION={FactoryGameVersion}");
    }

    private void AddSMLInfo()
    {
        // Get SML version from SML.uplugin
        var SMLPluginFile = FileReference.Combine(new DirectoryReference(PluginDirectory!), "SML.uplugin");
        var SMLPlugin = PluginDescriptor.FromFile(SMLPluginFile);
        PrivateDefinitions.Add($"SML_VERSION=\"{SMLPlugin.SemVersion}\"");
        
        //Collect build metadata from the environment and pass it to C++
        var CurrentBranch = Environment.GetEnvironmentVariable("BRANCH_NAME");
        var BuildId = Environment.GetEnvironmentVariable("BUILD_NUMBER");

        var GithubActionsWorkflow = Environment.GetEnvironmentVariable("GITHUB_WORKFLOW");
        if (!string.IsNullOrEmpty(GithubActionsWorkflow)) {
            CurrentBranch = Environment.GetEnvironmentVariable("GITHUB_REF_NAME");
            BuildId = Environment.GetEnvironmentVariable("GITHUB_RUN_NUMBER");
        }

        if (CurrentBranch == null) {
            RetrieveHeadBranchAndCommitFromGit(Target.ProjectFile!.Directory, out CurrentBranch, out BuildId);
            if (BuildId is { Length: > 8 }) {
                BuildId = BuildId[..8];
            }
        }
        if (CurrentBranch == null || BuildId == null) return;
        var BuildMetadataString = CurrentBranch == "master" ? BuildId : $"{CurrentBranch}.{BuildId}";
        PrivateDefinitions.Add($"SML_BUILD_METADATA=\"{BuildMetadataString}\"");
    }

    private static void RetrieveHeadBranchAndCommitFromGit(DirectoryReference RootDir, out string BranchName, out string CommitRef) {
        BranchName = null;
        CommitRef = null;

        var GitRepository = Path.Combine(RootDir.FullName, ".git");
        if (!Directory.Exists(GitRepository)) {
            return;
        }
        var GitHeadFile = Path.Combine(GitRepository, "HEAD");
        if (!File.Exists(GitHeadFile)) {
            return;
        }

        try {
            var HeadFileContents = File.ReadAllText(GitHeadFile).Replace("\n", "");

            //It is a normal HEAD ref, so its name should be equal to the local branch name
            if (HeadFileContents.StartsWith("ref: refs/heads/")) {
                BranchName = HeadFileContents.Substring(16);
                //Try to resolve commit name directly from the ref file
                var HeadRefFile = Path.Combine(GitRepository, "refs", "heads", BranchName);
                if (!File.Exists(HeadRefFile)) return;

                var HeadRefFileContents = File.ReadAllText(HeadRefFile).Replace("\n", "");
                CommitRef = HeadRefFileContents;
                return;
            }
            Log.TraceWarning("Git HEAD does not refer to a branch, are we in a detached head state? HEAD: {0}", HeadFileContents);
            BranchName = "detached-head";
            CommitRef = HeadFileContents;
        }
        catch (Exception Ex) {
            Log.TraceWarning("Failed to handle git HEAD file at {0}: {1}", GitHeadFile, Ex.Message);
        }
    }
}
