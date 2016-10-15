$(document).ready(function() {
    var client, player, remotePeer, initialized = false;

    //
    // Initialize the Symple WebRTC player

    player = new Symple.Player({
        element: '#webrtc-video .video-player',
        engine: 'WebRTC',
        rtcConfig: WEBRTC_CONFIG,
        mediaConstraints: {
            'mandatory': {
                // 'OfferToReceiveAudio': true,
                // 'OfferToReceiveVideo': true
            }
        },
        onStateChange: function(player, state) {
            player.displayStatus(state);
        }
    });

    function startPlaybackAndRecording() {
        // player.setup();
        player.play({ localMedia: true, disableAudio: false, disableVideo: false });
        player.engine.sendLocalSDP = function(desc) {
            console.log('Send offer:', JSON.stringify(desc))
            client.send({
                to: remotePeer,
                type: 'message',
                offer: desc
            });
        }

        player.engine.sendLocalCandidate = function(cand) {
            client.send({
                to: remotePeer,
                type: 'message',
                candidate: cand
            });
        }
    }

    //
    // Initialize the Symple client

    client = new Symple.Client(CLIENT_OPTIONS);

    client.on('announce', function(peer) {
        console.log('Authentication success:', peer);
    });

    client.on('addPeer', function(peer) {
        console.log('Adding peer:', peer);

        if (peer.user == 'videorecorder' &&
            !initialized) {
            initialized = true;
            remotePeer = peer; //m.from;
            startPlaybackAndRecording();
        }
    });

    client.on('removePeer', function(peer) {
        console.log('Removing peer:', peer);
    });

    client.on('message', function(m) {
        // console.log('Recv message:', m)
        if (remotePeer && remotePeer.id != m.from.id) {
            console.log('Dropping message from unknown peer', m);
            return;
        }
        if (m.offer) {
            alert('Unexpected offer for one-way streaming');
        }
        else if (m.answer) {
            console.log('Reieve answer:', JSON.stringify(m.answer));
            player.engine.recvRemoteSDP(m.answer);
        }
        else if (m.candidate) {
            player.engine.recvRemoteCandidate(m.candidate);
        }
    });

    // client.on('presence', function(p) {
    //     console.log('Recv presence:', p)
    // });

    // client.on('command', function(c) {
    //    console.log('Recv command:', c)
    // });

    // client.on('event', function(e) {
    //    console.log('Recv event:', e)
    // });

    client.on('disconnect', function() {
        console.log('Disconnected from server')
    });

    client.on('error', function(error, message) {
        console.log('Connection error:', error, message)
    });

    client.connect();
});
