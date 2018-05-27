
var agora_module = {

    AGORAEVT: {
        evt_tips: "msgTips", //< Tips message > 
        evt_local:"localVideo",       
        evt_remote:"remoteVideos",           
    },

    roomInput: null,
                          
    agoraVideoInst: null,
        
    addTips:function(strTips, errcode)
    {
        cc.log("[js] addTips : " +  strTips);
        var err = errcode != undefined? errcode: 0;
        var event = new cc.EventCustom(this.AGORAEVT.evt_tips);
        var p = {
            msg: strTips,
            error: err,
        };
        event.setUserData(p);
        cc.eventManager.dispatchEvent(event);
    },

    localNotify:function(channel, uid, elapsed)
    {
        var event = new cc.EventCustom(this.AGORAEVT.evt_local);
        var msg =  {
            channel: channel,
            uid : uid,
            elapsed : elapsed,
        };
        event.setUserData(msg);
        cc.eventManager.dispatchEvent(event);
    },

    remoteNotify:function(uid, width, height,elapsed)
    {
        var event = new cc.EventCustom(this.AGORAEVT.evt_remote);
        var msg =  {
            uid: uid,
            width : width,
            height : height,
            elapsed : elapsed,
        };
        
        event.setUserData(msg);
        cc.eventManager.dispatchEvent(event);
    },
    
    initAgoraVide:function(){

        this.addTips("初始化...");  
        if(this.agoraVideoInst == null)
        {
            this.agoraVideoInst = new AgoraVideo();
        }
        var self = this;
        if(this.agoraVideoInst != null)
        {
            this.agoraVideoInst.onJoinChannelSuccess = function(channel,  uid, elapsed){
                cc.log("[js] onJoinChannelSuccess, channel:%s,uid :%d, elapsed : %d !", channel, uid, elapsed);    
              
                self.addTips(" Join Channel Success !");
                self.localNotify(channel,  uid, elapsed);
            };

            this.agoraVideoInst.onLeaveChannel = function (totalDuration, txBytes, rxBytes,txKBitRate,rxKBitRate,txAudioKBitRate,rxAudioKBitRate,txVideoKBitRate,rxVideoKBitRate,users,cpuTotalUsage,cpuAppUsage){
                cc.log("[js]onLeaveChannel, totalDuration:%s,utxBytes :%d, rxBytes : %d !\n", totalDuration, txBytes, rxBytes);
            };

            this.agoraVideoInst.onFirstRemoteVideoDecoded = function(uid,  width,  height, elapsed){
                cc.log("[js]onLeaveChannel, uid:%d,width :%d, height :%d ,elapsed: %d!\n", uid, width, height,elapsed);
                self.remoteNotify(uid,  width,  height, elapsed);
            };

            //enable
            //  1: true
            //  0: false
            this.agoraVideoInst.onUserEnableVideo = function(uid, enable){
                cc.log("[js]onUserEnableVideo, uid:%d, enable: %d\n", uid, enable);
            };
        }
    },

    joinChannel:function(g_roomName, uid, videoEnabled, videoMode, info){
       
        if(this.agoraVideoInst == null){
            cc.log("agoraVideoInst  is null !");
            return false;
        }else {
         

        }

        if(this._remoteVideoSprite == null){
            this._remoteVideoSprite =  new Map(); 
        };  
    
        var errCode = this.agoraVideoInst.joinChannel(g_roomName, uid, videoEnabled, videoMode, info);
        if( 0  == errCode){
            cc.log("logIn Successfully !\n");
        }else {
            cc.log("logIn Failed, errCode：%d\n",  errCode);
        }
    },
                             
    leaveChannel:function(){
        var errCode = this.agoraVideo.leaveChannel();
        if( 0 == errCode){
            cc.log("leaveChannel is called Successfully !\n");
        }else {
            cc.log("leaveChannel is called Failed, errCode：%d\n",  errCode);
        }

        this.clearVideoSprites()                   
    },
    
    clearVideoSprites:function(){
                                      
        if (null != _localVideoSprite){
            this.removeChild(_localVideoSprite)
            this._localVideoSprite = null;
        }

        if (null != this._remoteVideoSprite){
            cc.log("_remoteVideoSprite.size : " + this._remoteVideoSprite.size);
                        
            for (var [key, value] of _remoteVideoSprite) {
                console.log(key + ' = ' + value);
                this.removeChild(value);
                this._remoteVideoSprite.delete(key);
            }
                                      
            cc.log("_remoteVideoSprite.size : " + this._remoteVideoSprite.size);
        }                             
    },                   
};

module.exports = agora_module;
