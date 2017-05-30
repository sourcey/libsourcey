#include "scy/socketio/socket.h"
#include "scy/socketio/transaction.h"
#include "scy/util.h"

#include <assert.h>
#include <conio.h>
#include <fstream>
#include <string>
#include <vector>


/* -----------------------------------------------------------------------------
 FIXME, THIS CODE IS OUT OF DATE!
----------------------------------------------------------------------------- */

using namespace std;
using namespace Sourcey;
using namespace Sourcey::Util;
using namespace Sourcey::Net;


class Messenger : public SocketIO::Socket
{
public:
    Messenger(const Net::Address& srvAddr)
        : SocketIO::Socket(srvAddr)
    {
    }

    ~Messenger() {}

    void sendTransaction(const SocketIO::Packet& packet)
    {
        SocketIO::Transaction* txn = new SocketIO::Transaction(*this, packet);
        txn->StateChanged +=
            sdelegate(this, &Messenger::onTransactionStateChanged);
        txn->send();
    }

    void onTransactionStateChanged(void* sender, TransactionState& state,
                                   const TransactionState&)
    {
        Log("debug", this) << "Transaction State Changed: " << state.toString()
                           << endl;

        SocketIO::Transaction* transaction =
            reinterpret_cast<SocketIO::Transaction*>(sender);
        switch (state.id()) {
            case TransactionState::Running:
                break;

            case TransactionState::Success:
                break;

            case TransactionState::Cancelled:
                break;

            case TransactionState::Failed:
                break;
        }
    };
};

//} // namespace SocketIO
} // namespace Sourcey


int main(int argc, char** argv)
{
    Logger::instance().add(new ConsoleChannel("debug", STraceevel));

    Sourcey::Net::Address srvAddr("localhost", 1337);
    Messenger app(srvAddr);
    app.connect();

    std::string network = "4";
    std::string event;
    char o = 0;
    while (o != 'Q') {
        try {
            cout << "COMMANDS:\n\n"
                    "  E    Set Event Name.\n"
                    "  A    Announce.\n"
                    "  1    Send Connect.\n"
                    "  M    Send Message.\n"
                    "  J    Send JSON Message.\n"
                    //"  X    Send XMPP Message.\n"
                    "  T    Send Transaction.\n"
                    //"  U    Call Upload Asset Service.\n"
                    //"  L    List Services.\n"
                    //"  K    List Remote Packages.\n"
                    //"  R    Reload Service List.\n"
                    //"  I    Install Packages.\n"
                    //"  U    Uninstall Packages.\n"
                    //"  D    Update All Packages.\n"
                    "  Q    Quit.\n\n";

            o = toupper(getch());

            // Set Input File
            if (o == 'E') {
                cout << "Enter event name: " << endl;
                getline(cin, event);
            }

            // Send Message
            else if (o == '1') {
                app.sendConnect();
            }

            // Send Message
            else if (o == 'M') {
                app.send(SocketIO::Packet::Message, "wassssupppp!", true);
            }

            // Send JSON Message
            else if (o == 'J') {
                app.send(SocketIO::Packet::JSON, "{\"name\":\"value\"}", true);
            }

            // Send Announce
            else if (o == 'A') {
                // app.emit("announce",
                // "{\"username\":\"admin\",\"name\":\"Admin
                // Dude\",\"account\":\"4\"}", true);
            }

            // Send XMPP Message
            else if (o == 'X') {
                // app.emit("xmpp", "{\"name\":\"value\"}", true);
            }

            // Send Transaction
            else if (o == 'T') {
                SocketIO::Packet request(SocketIO::Packet::Message, 99, "",
                                         "yooooooooooo", true);
                app.sendTransaction(request);
            }
        } catch (Poco::Exception& e) {
            cerr << e.displayText() << endl;
        }
    }


    return 0;
}


/*
// Call Upload Asset Service
else if (o == 'U') {

    Path file("D:/test.avi");

    HTMLForm* form = new HTMLForm(HTMLForm::ENCODING_MULTIPART_FORM);
    form->set("asset[type]", "Video");
    form->addPart("asset[file]", new FilePartSource(file.toString()));

    APITransaction* transaction = app.upload("UploadAsset", form);
//file.toString(),
    transaction->APITransactionComplete += sdelegate(&app,
&Messenger::onAPITransactionComplete);
    transaction->start();
}

// List Services
else if (o == 'L') {
    app.files().print(cout);
}

// Reload Service List
else if (o == 'R') {
    app.loadServices();
}
*/
/*
stringstream ss;
ss.put(0x00);
ss << "blah";
ss.put(0xff);
ss << "blah";
ss.put(0xff);
ss << "blah";
ss.put(0xff);

StringList v = Util::split(ss.str(), 0xff);
//cout <<  v.size();


stringstream ss1;
ss1 << 0x00;
ss1.write("1::", 3);
ss1 << 0xff;
cout <<  ss1.str();

stringstream ss2;
ss2.put(0x00);
ss2.write("1::", 3);
ss2.put(0xff);
cout <<  ss2.str();


system("pause");
return 0;
*/
