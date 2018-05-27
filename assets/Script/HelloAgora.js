var agoraVideo = require("agoravideo");

var _remoteVideoSprite = new Map();

var _localVideoSprite = null;
var _remoteVideoSprite = null;

cc.Class({
    extends: cc.Component,

    properties: {
        label: {
            default: null,
            type: cc.Label
        },

        lblroom: {
            default: null,
            type: cc.Label
        },

        channelName: {
            default: null,
            type: cc.EditBox
        },

        btnJoin: {
            default: null,
            type: cc.Button
        },

        btnLeave: {
            default: null,
            type: cc.Button
        },

        btnMic: {
            default: null,
            type: cc.Button
        },

        btnSpearker: {
            default: null,
            type: cc.Button
        },

        localSprite: {
            default: null,
            type: cc.Node,
        },

        //defaults, set visually when attaching this script to the Canvas
        text: 'Hello Agora !',

    },

    // use this for initialization
    onLoad: function () {        
        this.initUI();
        // this.initEvent();
        this.initEvent();

        this.label.string = this.text;
        
       
        cc.log("[onload] end");
    },


    initUI: function(){
        // this.updateUI(false);
    },

    initEvent:function(){      
        var mythis = this;
        var visibleSize = cc.director.getVisibleSize();
        cc.log("beck 2: " + agoraVideo.AGORAEVT.evt_tips);  
        var self = this;
        cc.eventManager.addListener({
            event: cc.EventListener.CUSTOM,   
            eventName: agoraVideo.AGORAEVT.evt_tips,            
            
            callback: function(event){
                cc.log(" beck : " + agoraVideo.AGORAEVT.evt_tips);
                var obj = event.getUserData();
                var s = obj.msg;
                if(obj.error != 0){
                    s += "[errorcode]" + obj.error;
                }
                cc.log("debug: " + s);
                self.label.string = s;
            }
        },this);

        cc.eventManager.addListener({
            event:cc.EventListener.CUSTOM,
            eventName:agoraVideo.AGORAEVT.evt_local,
            callback:function (event){
                cc.log("localVideo display ！\n");

                // _localVideoSprite = agoraVideo.agoraVideoInst.getLocalSprite();
                // _localVideoSprite.setPosition(cc.p(visibleSize.width/2, visibleSize.height/2));
                // mythis.addChild(_localVideoSprite);

                // this.localSprite.spriteFrame =_localVideoSprite;
                // this.node.addChild(_localVideoSprite);

                // var scene = cc.director.getScene();
                // var node = cc.instantiate(this._localVideoSprite);

                // node.parent = scene;
                // node.setPosition(0, 0);

            }
        },this);

        cc.eventManager.addListener({
            event:cc.EventListener.CUSTOM,
            eventName: agoraVideo.AGORAEVT.evt_remote,
            callback:function (event){
                agoraVideo.addTips("joinChannel Success !")
                var msg = event.getUserData();
                                                
                _remoteVideoSprite.set(msg.uid, agoraVideo.getRemoteSprites(msg.uid));
                _remoteVideoSprite.get(msg.uid).setPosition(cc.p((visibleSize.width/2 + 330), visibleSize.height/2));
                mythis.addChild(_remoteVideoSprite.get(msg.uid));
            }
        },this);

    },

    // called every frame
    update: function (dt) {

    },

    btnJoinRoomClick: function (event, customEventData) {
        agoraVideo.initAgoraVide();
        this.label.string = "正在加入房间...";
        var g_roomName = this.channelName.string;

        if(g_roomName == ""){
            cc.log("roomName is '' ");
            return false;
        }else {
            cc.log("roomName: " + g_roomName);
        }

        //agoraVideo
        /*!
         *  @param g_roomName   Channel name
         *  @param uid  user id.
         *  @param  videoEnabled   1: enable  video , 0: disable video
         *  @param  videoMode   one of VIDEO_PROFILE  in agora_cocos2dx.h
         *  @param  info   ""
         *  @return errCode
         */

        var uid = 0;
        var videoEnabled = 1;
        var videoMode = 35;
        var info ="";
        if(agoraVideo.agoraVideoInst == null){
            cc.log("agoraVideoInst should be not null.");
            return false;
        }else {
            cc.log("agoraVideoInst is not null.");
        }

        agoraVideo.agoraVideoInst.joinChannel(g_roomName, uid, videoEnabled, videoMode, info);
    },

    btnLeaveRoomClick: function (event, customEventData) {
        this.label.string ="正在离开房间...";
        agoraVideo.agoraVideoInst.leaveChannel();
    },  
});
