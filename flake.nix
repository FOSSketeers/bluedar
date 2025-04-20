{
  inputs.nixpkgs.url = "github:nixos/nixpkgs/nixpkgs-unstable";

  outputs = { nixpkgs, ... }:
  let
    forAllSystems = nixpkgs.lib.genAttrs [ "aarch64-linux" "x86_64-linux" ];
  in
  {
    devShell = forAllSystems (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in
      pkgs.mkShell rec {
        RUSTFLAGS = "-Clinker=clang";
        LD_LIBRARY_PATH = builtins.foldl' (a: b: "${a}:${b}/lib") "${pkgs.vulkan-loader}/lib" buildInputs;

        buildInputs = with pkgs; [
          # build
          cargo
          clang
          rustc

          #runtime
          expat
          fontconfig
          freetype
          libGL
          pkg-config
          xorg.libX11
          xorg.libXcursor
          xorg.libXi
          xorg.libXrandr
          wayland
          libxkbcommon
        ];
      }
    );
  };
}
