Please see http://sourcey.com/projects/libsourcey


EXTERNAL DEPENDENCIES
=================================================

OpennSSL >= 0.98
-------------------------------------------------


Poco >= 1.4
-------------------------------------------------

./configure --no-tests --no-samples --omit=Data,Data/SQLite,Data/MySQL,Data/ODBC,PageCompiler,PageCompiler/File2Page


OpenCV >= 2.3
-------------------------------------------------



FFmpeg >= 6
-------------------------------------------------


INTERNAL DEPENDENCIES
=================================================
LibSourcey also has a number of dependencies which we build ourselves.
Remember that dependencies must be built before the main project. 

PugiXML
RtAudio
libstrophe


OBTAINING SOURCES
=================================================
git clone https://bitbucket.org/sourcey/libsourcey.git



BUILDING
=================================================

Install CMake


Open CMake


Linux
-------------------------------------------------

1) Buld Dependencies

make install


2) Buld Modules

make install


3) Build Projects

make install