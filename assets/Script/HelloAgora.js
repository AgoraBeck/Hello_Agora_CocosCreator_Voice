var agoraAudio = require("agoraAudio");

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
        
        btnMuteLocal: {
            default: null,
            type: cc.Button
        },

        btnSpearker: {
            default: null,
            type: cc.Button
        },

        //defaults, set visually when attaching this script to the Canvas
        text: 'Hello Agora !',

        bMuteLocal:false,
        bSetSpeakerPhone:false,

    },

    // use this for initialization
    onLoad: function () {        

        this.initEvent();

        this.label.string = this.text;

        this.updateUI(false);
    },

    updateUI: function(bInited){
        this.btnJoin.interactable = !bInited;
        this.btnLeave.interactable = bInited;
        this.btnMuteLocal.interactable = bInited;
        this.btnSpearker.interactable = bInited;
    },

    // called every frame
    update: function (dt) {

    },

    initEvent:function(){      
        var mythis = this;
        
        var self = this;

        cc.eventManager.addListener({
            event: cc.EventListener.CUSTOM,   
            eventName: agoraAudio.AGORAEVT.evt_tips,            
            
            callback: function(event){
                var obj = event.getUserData();
                var s = obj.msg;
                if(obj.error != 0){
                    s += "[errorcode]" + obj.error;
                }
                self.label.string = s;
            }
        },this.node);

        cc.eventManager.addListener({
            event:cc.EventListener.CUSTOM,
            eventName:agoraAudio.AGORAEVT.evt_jSuccess,
            callback:function (event){
                self.updateUI(true);       
            }
        },this.node);

        cc.eventManager.addListener({
            event:cc.EventListener.CUSTOM,
            eventName: agoraAudio.AGORAEVT.evt_lSuccess,
            callback:function (event){    
                var msg = event.getUserData();
                self.updateUI(false);                                      
            }
        },this.node);

    },

    // called every frame
    update: function (dt) {

    },

    btnJoinRoomClick: function (event, customEventData) {
        agoraAudio.initAgoraAudio("4c51ad802859440cbfb89eb75919d9ed"); // input: APPID
        this.label.string = "正在加入房间...";
        var channelId = this.channelName.string;

        if(channelId == ""){
            agoraAudio.addTips("channelId is null."); 
            cc.log("channelId is '' ");
            return false;
        }else {
            cc.log("channelId: " + channelId);
        }

        //agoraAudio
        /*!
         *  @param  token   "" or token Value from App Server
         *  @param channelId   Channel name
         *  @param uid  user id, 0:SDK will generate it,or uid of app player's id
         *  @param  info   ""
         *  @return errCode
         */
        var token = "";
        var uid = 0;
        var info = "";
        if(agoraAudio.agoraAudioInst == null){
            cc.log("agoraAudioInst should be not null.");
            return false;
        }else {
            cc.log("agoraAudioInst is not null.");
        }
        agoraAudio.agoraAudioInst.joinChannel(token, channelId, info, uid);
    },

    btnLeaveRoomClick: function (event, customEventData) {
        this.label.string ="正在离开房间...";
        agoraAudio.agoraAudioInst.leaveChannel();
    },  

    btnMuteLocalClick: function (event, customEventData) {        
        if(!this.bMuteLocal){
            this.bMuteLocal = !this.bMuteLocal;
            agoraAudio.addTips("muteLocalAudioStream: " + this.bMuteLocal);                
        } else {
            this.bMuteLocal = !this.bMuteLocal; 
            agoraAudio.addTips("muteLocalAudioStream: " + this.bMuteLocal);            
        }
        agoraAudio.agoraAudioInst.muteLocalAudioStream(this.bMuteLocal);
    },

    btnSetSpeakerPhoneClick: function (event, customEventData) {
        if(!this.bSetSpeakerPhone){
            this.bSetSpeakerPhone = !this.bSetSpeakerPhone;
            agoraAudio.addTips("setEnableSpeakerphone: " + this.bSetSpeakerPhone);                
        } else {
            this.bSetSpeakerPhone = !this.bSetSpeakerPhone;
            agoraAudio.addTips("setEnableSpeakerphone: " + this.bSetSpeakerPhone);            
        }
        agoraAudio.agoraAudioInst.setEnableSpeakerphone(this.bSetSpeakerPhone);
       
    },
});