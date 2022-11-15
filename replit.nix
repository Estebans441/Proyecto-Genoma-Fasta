{ pkgs }: {
	deps = [
		pkgs.vlang
  pkgs.clang_12
		pkgs.ccls
		pkgs.gdb
		pkgs.gnumake
	];
}