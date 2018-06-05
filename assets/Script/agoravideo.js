
var agora_module = {

    AGORAEVT: {
        evt_tips: "msgTips", //< Tips message > 
        evt_jSuccess:"joinSuccess",       
        evt_lSuccess:"leaveSuccess",           
    },

    roomInput: null,
                          
    agoraVideoInst: null,
        
    addTips:function(strTips, errcode)
    {
        var err = errcode != undefined? errcode: 0;
        var event = new cc.EventCustom(this.AGORAEVT.evt_tips);
        var p = {
            msg: strTips,
            error: err,
        };
        event.setUserData(p);
        cc.eventManager.dispatchEvent(event);
    },

    jSuccessNotify:function(channel, uid, elapsed)
    {
        var event = new cc.EventCustom(this.AGORAEVT.evt_jSuccess);
        var msg =  {
            channel: channel,
            uid : uid,
            elapsed : elapsed,
        };
        event.setUserData(msg);
        cc.eventManager.dispatchEvent(event);
    },

    lSuccessNotify:function(uid, width, height,elapsed)
    {
        var event = new cc.EventCustom(this.AGORAEVT.evt_lSuccess);
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

        if(this.agoraVideoInst == null)
        {
            this.agoraVideoInst = new AgoraVideo();
        }
        var self = this;
        if(this.agoraVideoInst != null)
        {
            this.agoraVideoInst.onJoinChannelSuccess = function(channel,  uid, elapsed){
                cc.log("[js] onJoinChannelSuccess, channel:%s,uid :%d, elapsed : %d !", channel, uid, elapsed);    
              
                self.addTips(" Join Channel Successfully !");
                self.jSuccessNotify(channel,  uid, elapsed);
            };

            this.agoraVideoInst.onLeaveChannel = function (totalDuration, txBytes, rxBytes,txKBitRate,rxKBitRate,txAudioKBitRate,rxAudioKBitRate,txVideoKBitRate,rxVideoKBitRate,users,cpuTotalUsage,cpuAppUsage){
                cc.log("[js]onLeaveChannel, totalDuration:%s,utxBytes :%d, rxBytes : %d !\n", totalDuration, txBytes, rxBytes);
                self.addTips(" Leave Channel Successfully !");
                self.lSuccessNotify(totalDuration, txBytes, rxBytes,txKBitRate);
            };
        }
    },

    joinChannel:function(g_roomName, uid, videoEnabled, videoMode, info){
       
        if(this.agoraVideoInst == null){
            cc.log("agoraVideoInst  is null !");
            return false;
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
    },
};

module.exports = agora_module;
