# CMake build commands

## Doxygen

git clone https://github.com/doxygen/doxygen.git
git checkout tags/Release_1_8_12
cd doxygen
mkdir build
cd build
cmake -G "Unix Makefiles" ..
make
sudo make install

doxygen -g
INPUT=/home/kam/sourcey/libsourcey/src
OUTPUT_DIRECTORY=/home/kam/sourcey/libsourcey/build/doxygen
PROJECT_NAME="LibSourcey"
RECURSIVE=YES
GENERATE_XML=NO
GENERATE_HTML=NO
GENERATE_LATEX=NO
EXCLUDE_PATTERNS = */anionu* */test* */apps* */samples*

npm install doxygen2md
./node_modules/doxygen2md/index.js --verbose ./doc/xml > API.md

node ./doxygen2md/bin/doxygen2md.js --verbose ./biuld/doxygen/xml

node ./doxygen2md/bin/doxygen2md.js --verbose --groups --output=./doc ./build/doxygen/xml
node ./doxygen2md/bin/doxygen2md.js --verbose --groups --output ./doc/api.md ./build/doxygen/xml


TODO:
- split into namespaces
  - remove namespace list from beginning
- skip assertions
- disable Summary or Members
  - namespace heading h1
  - namespaces not generating preoprtly - linear + links broken
  - classes h1 to h2
  - members h3 to h4

npm install gitbook-cli
./node_modules/gitbook-cli/bin/gitbook.js init

https://github.com/contao/docs/blob/master/cookbook/book.json


funcs
replace '(    *.+);\n((        \/\/.*\n)+)' '$2\n$1;' . -r --include="*.h"
(    *.+);\n((        \/\/.*\n)+)
$2$1;\n

inline funcs
replace '(    [a-z]*.+\n)((        \/*.+\n)+)    {' '$2$1    {' ./uv -r --include="*.h"
(    [a-z]*.+\n)((        \/*.+\n)+)    {
$2$1    {


class
replace '// Module: (.*)\n//' '' . -r --include="*.h"
\n(class | struct )(.*\n)((    \/\/.*\n)+)
\n$3$1$2

## Default debug build
cmake .. -DCMAKE_BUILD_TYPE=DEBUG -DBUILD_SHARED_LIBS=OFF \
         -DBUILD_MODULES=ON -DBUILD_APPLICATIONS=ON \
         -DBUILD_SAMPLES=ON -DBUILD_TESTS=ON \
         -DWITH_FFMPEG=ON

## Default debug build (with WebRTC)
cmake .. -DCMAKE_BUILD_TYPE=DEBUG -DBUILD_SHARED_LIBS=ON \
        -DBUILD_MODULES=ON -DBUILD_APPLICATIONS=ON \
        -DBUILD_SAMPLES=ON -DBUILD_TESTS=ON \
        -DWITH_FFMPEG=ON -DWITH_WEBRTC=ON

## All modules (selective)
cmake .. -DCMAKE_BUILD_TYPE=DEBUG -DBUILD_SHARED_LIBS=OFF \
         -DBUILD_APPLICATIONS=OFF -DBUILD_MODULES=OFF \
         -DBUILD_SAMPLES=ON -DBUILD_TESTS=ON \
         -DWITH_FFMPEG=ON -DWITH_OPENCV=OFF \
         -DBUILD_MODULE_archo=ON \
         -DBUILD_MODULE_base=ON -DBUILD_MODULE_crypto=ON \
         -DBUILD_MODULE_http=ON -DBUILD_MODULE_json=ON \
         -DBUILD_MODULE_av=ON -DBUILD_MODULE_net=ON \
         -DBUILD_MODULE_pacm=ON -DBUILD_MODULE_pluga=ON \
         -DBUILD_MODULE_sked=ON -DBUILD_MODULE_socketio=ON \
         -DBUILD_MODULE_stun=ON -DBUILD_MODULE_symple=ON \
         -DBUILD_MODULE_turn=ON -DBUILD_MODULE_util=ON \
         -DBUILD_MODULE_uv=ON -DBUILD_MODULE_webrtc=ON

## Minimum build (uv, base)
cmake .. -DCMAKE_BUILD_TYPE=DEBUG -DBUILD_MODULES=OFF \
         -DBUILD_MODULE_base=ON -DBUILD_MODULE_uv=ON

## Media build
cmake .. -DCMAKE_BUILD_TYPE=DEBUG -DBUILD_SHARED_LIBS=OFF \
         -DBUILD_APPLICATIONS=OFF -DBUILD_MODULES=OFF \
         -DBUILD_SAMPLES=ON -DBUILD_TESTS=ON \
         -DWITH_FFMPEG=ON -DWITH_OPENCV=OFF \
         -DBUILD_MODULE_av=ON -DBUILD_MODULE_base=ON \
         -DBUILD_MODULE_uv=ON

## Symple build (no media)
cmake .. -DCMAKE_BUILD_TYPE=DEBUG -DBUILD_SHARED_LIBS=OFF \
         -DBUILD_APPLICATIONS=ON -DBUILD_MODULES=OFF \
         -DBUILD_SAMPLES=ON -DBUILD_TESTS=ON \
         -DWITH_FFMPEG=ON -DWITH_OPENCV=OFF \
         -DBUILD_MODULE_archo=ON -DBUILD_MODULE_base=ON \
         -DBUILD_MODULE_crypto=ON -DBUILD_MODULE_http=ON \
         -DBUILD_MODULE_json=ON -DBUILD_MODULE_net=ON \
         -DBUILD_MODULE_socketio=ON -DBUILD_MODULE_symple=ON \
         -DBUILD_MODULE_util=ON -DBUILD_MODULE_uv=ON

## Symple build (with media)
cmake .. -DCMAKE_BUILD_TYPE=DEBUG -DBUILD_SHARED_LIBS=OFF \
         -DBUILD_APPLICATIONS=ON -DBUILD_MODULES=OFF \
         -DBUILD_SAMPLES=ON -DBUILD_TESTS=ON \
         -DWITH_FFMPEG=ON -DWITH_OPENCV=OFF \
         -DBUILD_MODULE_archo=ON -DBUILD_MODULE_base=ON \
         -DBUILD_MODULE_crypto=ON -DBUILD_MODULE_http=ON \
         -DBUILD_MODULE_json=ON -DBUILD_MODULE_av=ON \
         -DBUILD_MODULE_net=ON -DBUILD_MODULE_socketio=ON \
         -DBUILD_MODULE_symple=ON -DBUILD_MODULE_util=ON \
         -DBUILD_MODULE_uv=ON

## HTTP build
cmake .. -DCMAKE_BUILD_TYPE=DEBUG -DBUILD_SHARED_LIBS=OFF \
         -DBUILD_APPLICATIONS=ON -DBUILD_MODULES=OFF \
         -DBUILD_SAMPLES=ON -DBUILD_TESTS=ON \
         -DBUILD_MODULE_archo=ON -DBUILD_MODULE_base=ON \
         -DBUILD_MODULE_crypto=ON -DBUILD_MODULE_http=ON \
         -DBUILD_MODULE_json=ON -DBUILD_MODULE_net=ON \
         -DBUILD_MODULE_util=ON -DBUILD_MODULE_uv=ON

## STUN/TURN build
cmake .. -DCMAKE_BUILD_TYPE=DEBUG -DBUILD_SHARED_LIBS=OFF \
         -DBUILD_APPLICATIONS=ON -DBUILD_MODULES=OFF \
         -DBUILD_MODULE_base=ON -DBUILD_MODULE_crypto=ON \
         -DBUILD_MODULE_net=ON -DBUILD_MODULE_stun=ON \
         -DBUILD_MODULE_turn=ON -DBUILD_MODULE_util=OFF \
         -DBUILD_MODULE_uv=ON
