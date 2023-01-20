using System.Diagnostics;
using Reloaded.Injector;

const string InjectLibraryC = "InjectLibraryC.dll";
const string InjectLibraryStandardBootstrapper = "InjectLibraryStandard.Bootstrapper.dll";
const string InjectLibraryNETBootstrapper = "InjectLibraryNET.Bootstrapper.dll";

var process = Process.Start(new ProcessStartInfo
{
    FileName = "mspaint.exe"
});

//var process = Process.GetProcessesByName("ac_client").FirstOrDefault();

var workingLibrary = Path.GetFullPath(Path.Combine(Directory.GetCurrentDirectory(), @"..\", InjectLibraryNETBootstrapper));
var loadedLibrary = Path.GetFullPath(Path.Combine(Directory.GetCurrentDirectory(), @"..\", "InjectLibraryNET.dll"));

var injector = new Injector(process);

var result = injector.Inject(workingLibrary);


if (result != 0)
{
    //injector.Eject(workingLibrary);

    // bootstrapper cleans himself, but we should eject C# DLL
    injector.Eject(loadedLibrary);
}

injector.Dispose();

process?.Kill();
process?.Dispose();