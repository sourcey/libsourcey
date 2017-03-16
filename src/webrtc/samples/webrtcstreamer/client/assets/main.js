$(document).ready(function() {
    var client, player, remotePeer;

    //
    // Initialize the Symple WebRTC player

    player = new Symple.Player({
        element: '#webrtc-video .video-player',
        engine: 'WebRTC',
        rtcConfig: WEBRTC_CONFIG,
        iceMediaConstraints: {
            'mandatory': {
                'OfferToReceiveAudio': true,
                'OfferToReceiveVideo': true
            }
        },
        onStateChange: function(player, state) {
            player.displayStatus(state);
        }
    });

    //
    // Initialize the Symple client

    client = new Symple.Client(CLIENT_OPTIONS);

    client.on('announce', function(peer) {
        console.log('Authentication success:', peer);
    });

    client.on('presence', function(p) {
        console.log('Recv presence:', p)
    });

    client.on('message', function(m) {
        // console.log('Recv message:', m)
        if (remotePeer && remotePeer.id != m.from.id) {
            console.log('Dropping message from unknown peer', m);
            return;
        }

        if (m.offer) {
            try {
                console.log('Reieve offer:', JSON.stringify(m.offer))

                remotePeer = m.from;
                // player.setup();
                player.play();
                player.engine.recvRemoteSDP(m.offer);
                player.engine.sendLocalSDP = function(desc) {
                    console.log('Send answer:', JSON.stringify(desc))
                    client.send({
                        to: remotePeer,
                        type: 'message',
                        answer: desc
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
            catch (e) {
                console.log("Failed to create PeerConnection:", e);
            }
        }

        else if (m.answer) {
            alert('Unexpected answer for one-way streaming');
        }

        else if (m.candidate) {
            // Filter non relay candidates for testing TURN
            // if (m.candidate.candidate.indexOf('typ relay') == -1) { //typ host
            //    console.log("Dropping Candidate:", m.candidate);
            //    return;
            // }

            // console.log("Using Candidate:", m.candidate);
            player.engine.recvRemoteCandidate(m.candidate);
        }
    });

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

    client.on('addPeer', function(peer) {
        // console.log('Adding peer:', peer)
    });

    client.on('removePeer', function(peer) {
        console.log('Removing peer:', peer)
        if (remotePeer && remotePeer.id == peer.id) {
            remotePeer = null;
        }
    });

    client.connect();
});
