This is strophe, our XMPP client library.

Our goals are:

    * usable quickly
    * well documented
    * reliable

== GIT Instructions ==

By default, libstrophe has a dependency on the XML parsing
library expat.  Expat is included as a submodule of this
repository.  After cloning this repository, you will need
to run the following to acquire the expat submodule:

$ git submodule init

$ git submodule update

== Build Instructions ==

From the top-level directory, run the following commands

NOTE: By default libstrophe uses expat as it's XML parser.
You may pass in --with-libxml2 with the ./configure command
to switch to using libxml2 as your XML parser.

$ ./bootstrap.sh

$ ./configure
or
$ ./configure --with-libxml2

$ make

This will create a static library, also in the top-level
directory, which can be linked into other programs. The 
public api is defined in <strophe.h> which is also in the
top-level directory.

The examples/ directory contains some examples of how to
use the library; these may be helpful in addition to the
API documentation in doc/.


== Requirements ==

Libstrophe requires libresolv on UNIX systems. Make sure you include -lresolv
if you are compiling by hand.

It also uses expat for XML processing, but a current copy is included in the
expat/ directory of the SVN checkout


