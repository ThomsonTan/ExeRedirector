# ExeRedirector
Redirect exe name to another command line when launching
------
A sample usage is the external diff tool for gitk doesn't accept any extra command line parameters on Windows. Make this exe as diff tool and save the real command line to config file (ExeRedirector.exe.config). The command line parameters for this tool will be appended to the commmand line extracted from config file.

The exe file name can be renamed at will, and config file name is appending ".config" to it.

I added below line to config file for vim diff.

`gvim.exe -d -c"norm ]c[c"`

------
Build: run `make.cmd` in VS command prompt.

