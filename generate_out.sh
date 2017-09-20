rm *.tar.gz
mkdir out
mkdir out/src
mkdir out/doc
mkdir out/build
cp README.md out/
cp CMakeLists.txt out/
cp src/* out/src/
cp doc/*.pdf out/doc/
cp build/Debug/* out/build/
tar -czf out.tar.gz out/
rm -r out/
