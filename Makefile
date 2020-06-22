msw:
	del /q build\*
	g++ -m32 -static -D__WXMSW__ -DwxUSE_UNICODE -DWIN32_LEAN_AND_MEAN -Wall -std=c++11 -O2 -pipe -mthreads -Wno-deprecated-declarations -Iinclude -c serialGUIApp.cpp -o build\serialGUIApp.o
	g++ -m32 -static -D__WXMSW__ -DwxUSE_UNICODE -DWIN32_LEAN_AND_MEAN -Wall -std=c++11 -O2 -pipe -mthreads -Wno-deprecated-declarations -Iinclude -c serialGUIMain.cpp -o build\serialGUIMain.o
	g++ -m32 -static -D__WXMSW__ -DwxUSE_UNICODE -DWIN32_LEAN_AND_MEAN -Wall -std=c++11 -O2 -pipe -mthreads -Wno-deprecated-declarations -Iinclude -c Serial_data.cpp -o build\Serial_data.o
	g++ -m32 -Llib\i386 build\serialGUIApp.o build\serialGUIMain.o build\Serial_data.o include\res\i386\resource.res -mthreads -lwxmsw30u_core -lwxbase30u -lwxpng -lwxzlib -lkernel32 -luser32 -lgdi32 -lwinspool -lcomdlg32 -ladvapi32 -lshell32 -lole32 -loleaut32 -luuid -lcomctl32 -lwsock32 -lodbc32 -lmathplot -lws2_32 -mwindows -o build\serialGUI.exe
	strip build\serialGUI.exe
	del /q build\*.o
msw_x86_64:
	del /q build\*
	g++ -m64 -static -D__WXMSW__ -DwxUSE_UNICODE -DWIN32_LEAN_AND_MEAN -Wall -std=c++11 -O2 -pipe -mthreads -Wno-deprecated-declarations -Iinclude -c serialGUIApp.cpp -o build\serialGUIApp.o
	g++ -m64 -static -D__WXMSW__ -DwxUSE_UNICODE -DWIN32_LEAN_AND_MEAN -Wall -std=c++11 -O2 -pipe -mthreads -Wno-deprecated-declarations -Iinclude -c serialGUIMain.cpp -o build\serialGUIMain.o
	g++ -m64 -static -D__WXMSW__ -DwxUSE_UNICODE -DWIN32_LEAN_AND_MEAN -Wall -std=c++11 -O2 -pipe -mthreads -Wno-deprecated-declarations -Iinclude -c Serial_data.cpp -o build\Serial_data.o
	g++ -m64 -Llib\x86_64 build\serialGUIApp.o build\serialGUIMain.o build\Serial_data.o include\res\x86_64\resource.res -mthreads -lwxmsw30u_core -lwxbase30u -lwxpng -lwxzlib -lkernel32 -luser32 -lgdi32 -lwinspool -lcomdlg32 -ladvapi32 -lshell32 -lole32 -loleaut32 -luuid -lcomctl32 -lwsock32 -lodbc32 -lmathplot -lws2_32 -mwindows -o build\serialGUI.exe
	strip build\serialGUI.exe
	del /q build\*.o
linux:
	rm -f build\*
	g++ -m64 -static -D__WXMSW__ -DwxUSE_UNICODE -DWIN32_LEAN_AND_MEAN -Wall -std=c++11 -O2 -pipe -mthreads -Wno-deprecated-declarations -c serialGUIApp.cpp -o build\serialGUIApp.o
	g++ -m64 -static -D__WXMSW__ -DwxUSE_UNICODE -DWIN32_LEAN_AND_MEAN -Wall -std=c++11 -O2 -pipe -mthreads -Wno-deprecated-declarations -c serialGUIMain.cpp -o build\serialGUIMain.o
	g++ -m64 -static -D__WXMSW__ -DwxUSE_UNICODE -DWIN32_LEAN_AND_MEAN -Wall -std=c++11 -O2 -pipe -mthreads -Wno-deprecated-declarations -c Serial_data.cpp -o build\Serial_data.o
	g++ -m64 -Llib\x86_64 build\serialGUIApp.o build\serialGUIMain.o build\Serial_data.o include\res\x86_64\resource.res -mthreads -lwxmsw30u_core -lwxbase30u -lwxpng -lwxzlib -lkernel32 -luser32 -lgdi32 -lcomdlg32 -ladvapi32 -lshell32 -lole32 -loleaut32 -luuid -lodbc32 -lmathplot -o build\serialGUI.exe
	strip build\serialGUI.exe
	rm -f build\*.o
clean:
	del /q build\*
