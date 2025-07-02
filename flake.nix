{
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
    }:

    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
        buildInputs = with pkgs; [
          qt6.full
          flex
          bison
          graphviz
        ];
      in
      {
        devShells.default = pkgs.mkShell {
          nativeBuildInputs = buildInputs;
          packages = with pkgs; [
            cmake
            clang
            qtcreator
          ];
        };
      }
    );
}
