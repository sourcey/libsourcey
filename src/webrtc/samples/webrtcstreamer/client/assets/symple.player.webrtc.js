// -----------------------------------------------------------------------------
// WebRTC Engine
//

window.RTCPeerConnection = window.mozRTCPeerConnection || window.webkitRTCPeerConnection;
window.RTCSessionDescription = window.mozRTCSessionDescription || window.RTCSessionDescription;
window.RTCIceCandidate = window.mozRTCIceCandidate || window.RTCIceCandidate;
window.URL = window.webkitURL || window.URL;
navigator.getUserMedia = navigator.getUserMedia || navigator.webkitGetUserMedia || navigator.mozGetUserMedia || navigator.msGetUserMedia;


Symple.Media.registerEngine({
    id: 'WebRTC',
    name: 'WebRTC Player',
    formats: 'VP8, Opus',
    preference: 100,
    support: (function() {
        return typeof RTCPeerConnection != 'undefined';
    })()
});


Symple.Player.Engine.WebRTC = Symple.Player.Engine.extend({
    init: function(player) {
        Symple.log('symple:webrtc: init');
        this._super(player);

        this.rtcConfig = player.options.rtcConfig || {
          iceServers: [
            { url: 'stun:stun.l.google.com:19302' }
          ]
        }
        this.rtcOptions = player.options.rtcOptions || {
            optional: [
                { DtlsSrtpKeyAgreement: true } // required for FF <=> Chrome interop
            ]
        }
        this.mediaConstraints = player.options.mediaConstraints || {}
    },

    setup: function() {
        Symple.log('symple:webrtc: setup');

        this._createPeerConnection();

        if (typeof(this.video) == 'undefined') {
            this.video = document.createElement('video');
            this.video.autoplay = true;
            this.player.screen.prepend(this.video);
        }
    },

    destroy: function() {
        Symple.log('symple:webrtc: destroy');

        this.sendLocalSDP = null;
        this.sendLocalCandidate = null;

        if (this.video) {
            this.video.src = '';
            this.video = null;
            // Anything else required for video cleanup?
        }

        if (this.pc) {
            this.pc.close();
            this.pc = null;
            // Anything else required for peer connection cleanup?
        }
    },

    play: function(params) {
        Symple.log('symple:webrtc: play', params);

        // NOTE: The 'playing' state will only be set when candidate gathering
        // is complete.

        // if `params.localMedia` is set then display the local video stream.
        if (params && params.localMedia) {
            var self = this;

            // TODO: Support device enumeration.
            navigator.getUserMedia({ audio: !params.disableAudio, video: !params.disableVideo },
                function (localStream) { // success

                    // Play the local video stream and create the SDP offer.
                    self.video.src = URL.createObjectURL(localStream);
                    self.pc.addStream(localStream);
                    self.pc.createOffer(function(desc) { self._onLocalSDP(desc); });
                },
                function(err) { // error
                    self.setError('getUserMedia() Failed: ' + err);
                });
        }
    },

    stop: function() {
        if (this.video) {
            this.video.src = '';
            // Do not nullify
        }

        // TODO: Close peer connection?
        if (this.pc) {
            this.pc.close();
            this.pc = null;
        }

        this.setState('stopped');
    },

    mute: function(flag) {
        // Mute unless explicit false given
        flag = flag === false ? false : true;

        Symple.log('symple:webrtc: mute:', flag);

        if (this.video)
            this.video.prop('muted', flag);
    },

    // Called when local SDP is ready to be sent to the peer.
    sendLocalSDP: new Function,

    // Called when a local candidate is ready to be sent to the peer.
    sendLocalCandidate: new Function,

    // Called when remote SDP is received from the peer.
    recvRemoteSDP: function(desc) {
        Symple.log('symple:webrtc: recv remote sdp:', desc)
        if (!desc || !desc.type || !desc.sdp)
            throw 'Invalid remote SDP';

        // if (desc.type != 'offer')
        //    throw 'Only SDP offers are supported'

        var self = this;
        this.pc.setRemoteDescription(new RTCSessionDescription(desc),
            function() {
                Symple.log('symple:webrtc: sdp success');
            },
            function(message) {
                console.error('symple:webrtc: sdp error:', message);
                self.setError('Cannot parse remote SDP offer');
            }
        );

        if (desc.type == 'offer') {
            self.pc.createAnswer(
                function(answer) { // success
                    self._onLocalSDP(answer);
                },
                function() { // error
                    self.setError('Cannot create local SDP answer');
                },
                null // this.mediaConstraints
            );
        }
    },

    // Called when remote candidate is received from the peer.
    recvRemoteCandidate: function(candidate) {
        Symple.log('symple:webrtc: recv remote candiate ', candidate);
        if (!this.pc)
            throw 'The peer connection is not initialized'; // call recvRemoteSDP first

        this.pc.addIceCandidate(new RTCIceCandidate(candidate));
    },

    //
    // Private methods
    //

    // Called when local SDP is ready to be sent to the peer.
    _onLocalSDP: function(desc) {
        try {
            this.pc.setLocalDescription(desc);
            this.sendLocalSDP(desc);
        }
        catch (e) {
            Symple.log('Failed to send local SDP:', e);
        }
    },

    // Create the RTCPeerConnection object.
    _createPeerConnection: function() {
        if (this.pc)
            throw 'The peer connection is already initialized';

        Symple.log('symple:webrtc: create RTCPeerConnnection with config: ',
            JSON.stringify(this.rtcConfig), JSON.stringify(this.rtcOptions));

        var self = this;
        this.pc = new RTCPeerConnection(this.rtcConfig, this.rtcOptions);
        this.pc.onicecandidate = function(event) {
            if (event.candidate) {
                Symple.log('symple:webrtc: local candidate gathered:', event.candidate);
                self.sendLocalCandidate(event.candidate);
            }
            else {
                Symple.log('symple:webrtc: local candidate gathering complete');
            }
        };
        this.pc.onaddstream = function(event) {
            Symple.log('symple:webrtc: remote stream added:', URL.createObjectURL(event.stream));

            // Set the state to playing once candidates have completed gathering.
            // This is the best we can do until ICE onstatechange is implemented.
            self.setState('playing');

            self.video.src = URL.createObjectURL(event.stream);
            self.video.play();
        };
        this.pc.onremovestream = function(event) {
            Symple.log('symple:webrtc: remote stream removed:', event);
            self.video.stop();
            self.video.src = '';
        };

        // NOTE: The following state events are still very unreliable.
        // Hopefully when the spec is complete this will change, but until then
        // we need to 'guess' the state.
        // this.pc.onconnecting = function(event) { Symple.log('symple:webrtc: onconnecting:', event); };
        // this.pc.onopen = function(event) { Symple.log('symple:webrtc: onopen:', event); };
        // this.pc.onicechange = function(event) { Symple.log('symple:webrtc: onicechange :', event); };
        // this.pc.onstatechange = function(event) { Symple.log('symple:webrtc: onstatechange :', event); };
    }
});


//
// Helpers

Symple.Media.iceCandidateType = function(candidateSDP) {
    if (candidateSDP.indexOf('typ relay') != -1)
        return 'turn';
    if (candidateSDP.indexOf('typ srflx') != -1)
        return 'stun';
    if (candidateSDP.indexOf('typ host') != -1)
        return 'host';
    return 'unknown';
}
