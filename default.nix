{ pkgs ? import <nixpkgs> {} }:
let
  stdenv = pkgs.stdenv;
in rec {
  utility = stdenv.mkDerivation {
    name = "utility";
    nativeBuildInputs = [
      pkgs.cmake
    ];
    buildInputs = [
      pkgs.openssl
      pkgs.gtest
    ];
  };
}
