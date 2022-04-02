d:
	ninja -C build/debug -f build.ninja
	./build/debug/2DShadowTransformation.exe

b:
	cmake -Bbuild/debug -GNinja -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ .
	cp build/debug/compile_commands.json .
