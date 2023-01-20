using System;
using System.Runtime.InteropServices;

namespace InjectLibraryStandard
{
    public class MyClass
    {
        [DllImport("User32.dll", CharSet = CharSet.Unicode)]
        private static extern int MessageBox(IntPtr h, string m, string c, int type);

        // This method will be called by native code inside the target process…
        public static int MyMethod(string pwzArgument)
        {
            // show modal message box
            MessageBox(IntPtr.Zero,
                ".DotNet 4.8 Managed App Injected Successfully from managed c++.\n\n" +
                "Running Inside: [" + System.Diagnostics.Process.GetCurrentProcess().ProcessName + "] Process\n\n" +
                (string.IsNullOrEmpty(pwzArgument) ? "No Argument Received" : "Received Argument: [" + pwzArgument + "]")
                , "", 0);
            return 0;
        }
    }
}
