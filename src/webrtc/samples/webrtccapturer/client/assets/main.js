$(document).ready(function() {
    var client, player, remotePeer, remoteStreamingDevice;

    //
    // WebRTC player

    function recreatePlayer() {
        console.log('Recreate player:', player);
        destroyPlayer();

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
        player.setup();
    }

    function destroyPlayer() {
        if (player) {
            player.destroy()
            player = null;
        }
    }


    //
    //= Commands

    // Get a list of streaming devices from the peer
    var refreshStreamingDevices = function(peer) {
        client.sendCommand({
            node: 'streaming:devices',
            to: peer
        });
    };

    // Start streaming video from the peer
    var startStreaming = function(peer, device) {
        recreatePlayer();

        remoteStreamingDevice = device;
        client.sendCommand({
            node: 'streaming:start',
            to: peer,
            data: {
                device: device
            }
        });
    };

    // Stop streaming video from the peer
    var stopStreaming = function(peer, device) {
        destroyPlayer();

        remoteStreamingDevice = null;
        client.sendCommand({
            node: 'streaming:stop',
            to: peer,
            data: {
                device: device
            }
        });
    };


    //
    // Bind UI events

    $('#streaming-devices').on('click', 'a', function(event) {
        var $this = $(this),
            user = $this.data('user'),
            device = $this.data('device'),
            isActive = $this.hasClass('active');
        if (isActive) {
            $this.removeClass('active');
            stopStreaming(user, device);
        }
        else {
            $this.addClass('active').siblings().removeClass('active');
            startStreaming(user, device);
        }

        event.preventDefault();
    });


    //
    // Symple client

    client = new Symple.Client(CLIENT_OPTIONS);

    client.on('announce', function(peer) {
        // console.log('Authentication success:', peer);
    });

    client.on('presence', function(p) {
        // console.log('Recv presence:', p);

        // Handle presence packets from peers
    });

    client.on('message', function(m) {
        // console.log('Recv message:', m);

        // Handle messages from peers
    });

    client.on('command', function(c) {
        // console.log('Recv command:', c)

        if (remotePeer && remotePeer.id != c.from.id) {
            console.log('Dropping message from unknown peer', m);
            return;
        }

        if (c.node == 'streaming:start') {
            if (c.status == 200) {
                // Streaming start success response
                // TODO: Update button state?
                // createPlayer();
            }
            else {
                // Command error
            }
        }

        else if (c.node == 'streaming:devices') {
            if (c.status == 200) {

                // Add new devices to the list
                var $devs = $('#streaming-devices');
                for (var i = 0; i < c.data.devices.length; i++) {
                    var dev = c.data.devices[i];
                    if (!$devs.find('[data-device="' + dev + '"]').length)
                        $devs.append('<a href="#" data-user="' + c.from.user + '" data-device="' + dev + '" ' +
                            'class="list-group-item list-group-item-action">' + c.from.user + ': ' + dev + '</a>');
                }
            }
            else {
                // Command error
            }
        }

        else if (c.node == 'streaming:files') {
            // TODO: file streaming
        }
    });

    client.on('event', function(e) {
        // console.log('Recv event:', e)

        // Just handle events from he current streaming peer
        // for the porpose of this demo
        if (remotePeer && remotePeer.id != e.from.id) {
            console.log('Dropping message from unknown peer', m);
            return;
        }

        // ICE SDP
        if (e.name == 'ice:sdp') {
             try {
                console.log('Reieve offer:', e.sdp);

                remotePeer = e.from;
                player.play();
                player.engine.recvRemoteSDP(e.sdp);
                player.engine.sendLocalSDP = function(desc) {
                    console.log('Send answer:', desc)
                    client.send({
                        to: remotePeer,
                        name: 'ice:sdp',
                        type: 'event',
                        sdp: desc
                    });
                }

                player.engine.sendLocalCandidate = function(cand) {
                    client.send({
                        to: remotePeer,
                        name: 'ice:candidate',
                        type: 'event',
                        candidate: cand
                    });
                }
            }
            catch (e) {
                console.log("Failed to create PeerConnection:", e);
            }

            // if (e.sdp.type == 'offer') {

            //     // Create the remote player on offer
            //     if (!$scope.remotePlayer) {
            //         $scope.remotePlayer = createPlayer($scope, 'answerer', '#video .remote-video');
            //         $scope.remotePlayer.play();
            //     }
            //     $scope.remotePlayer.engine.recvRemoteSDP(e.sdp);
            // }
            // if (e.sdp.type == 'answer') {
            //     $scope.localPlayer.engine.recvRemoteSDP(e.sdp);
            // }
        }

        // ICE Candidate
        else if (e.name == 'ice:candidate') {
            console.log('Recreate player:', player);
            player.engine.recvRemoteCandidate(e.candidate);

            // if (e.origin == 'answerer')
            //     $scope.localPlayer.engine.recvRemoteCandidate(e.candidate);
            // else //if (e.origin == 'caller')
            //     $scope.remotePlayer.engine.recvRemoteCandidate(e.candidate);
            // // else
            //     alert('Unknown candidate origin');
        }

        else {
            alert('Unknown event: ' + e.name);
        }
    });

    // client.on('event', function(e) {
    //    console.log('Recv event:', e)
    // });

    client.on('disconnect', function() {
        // console.log('Disconnected from server')
    });

    client.on('error', function(error, message) {
        // console.log('Peer error:', error, message)
    });

    client.on('addPeer', function(peer) {
        // console.log('Adding peer:', peer)

        // Get a list of streaming devices as soon as the peer connects
        if (peer.type == 'demo') {
            refreshStreamingDevices(peer);
        }
    });

    client.on('removePeer', function(peer) {
        // console.log('Removing peer:', peer)
        $('[data-user="' + peer.user + '"]').remove();
        if (remotePeer && remotePeer.id == peer.id) {
            remotePeer = null;
            destroyPlayer();
        }
    });

    client.connect();
});
