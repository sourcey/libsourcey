# Symple WebRTC Video Chat Demo

The Symple video chat demo is an example of how to use the Symple for instant messaging and WebRTC signaling in about 100 lines of JavaScript. External projects used are AngularJS, Bootstrap, Node.js and Express.

See this blog post for more information about the demo: http://sourcey.com/symple-webrtc-video-chat-demo

## What is Symple?

Symple is a unrestrictive real-time messaging and presence protocol.

The protocol itself is semantically similar to XMPP, except that it is much more flexible and economical due to the use of JSON instead of XML for encoding messages.

Symple currently has client implementations in [JavaScript](https://github.com/sourcey/symple-client), [Ruby](https://github.com/sourcey/symple-client-ruby) and [C++](https://github.com/sourcey/libsourcey/tree/master/src/symple), which make it ideal for a wide range of messaging requirements, such as building real-time games and applications which run in the web browser, desktop, and mobile phone.

## Usage

1. Install dependencies: `npm install`
2. Fire up the server: `node app`
3. And point your browser to: `http://localhost:4500`

## Hacking

Some key options are specified in the main HTML file located at `index.ejs`

**CLIENT_OPTIONS** This is the options hash for the Symple client. This is where you specify the server URL and Peer object. Note that we have disabled 'websocket' transport by default, but you will probably want to re-enable it in production.

**WEBRTC_CONFIG** This is the PeerConnection options hash. In production you will want to specify some TURN servers so as to ensure the p2p connection succeeds in all network topologies.

Other than that all relevant JavaScript is located in `public/js/app.js` and `public/js/helpers.js`. Enjoy!

## Contributing

1. Fork it
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Add some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create new Pull Request

## Contact

For more information please check out the Symple homepage: http://sourcey.com/symple/
If you have a bug or an issue then please use the Github issue tracker: https://github.com/sourcey/symple-client/issues
