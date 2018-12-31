
var agoraAudio_module = {

    AGORAEVT: {
        evt_tips: "msgTips", //< Tips message > 
        evt_jSuccess:"joinSuccess",       
        evt_lSuccess:"leaveSuccess",           
    },

    roomInput: null,
                          
    agoraAudioInst: null,
        
    addTips:function(strTips, errcode)
    {
        var err = errcode != undefined? errcode: 0;
        // var event = new cc.EventCustom(this.AGORAEVT.evt_tips);
        // var p = {
        //     msg: strTips,
        //     error: err,
        // };
        // event.setUserData(p);
        // cc.eventManager.dispatchEvent(event);

        let event = new cc.Event(this.AGORAEVT.evt_tips, true);
        var p = {
            msg: strTips,
            error: err,
        };
        event["data"] = p;
        cc.systemEvent.dispatchEvent(event);

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
    
    createEngine:function(appid){

        if(this.agoraAudioInst == null)
        {
            cc.log("[js] new agoraAudio()" )
            this.agoraAudioInst = new agoraAudio();
        }
        var self = this;
        if(this.agoraAudioInst != null)
        {
            cc.log("[js] agoraAudioInst.initialize()" )
            this.agoraAudioInst.initialize(appid)
            this.agoraAudioInst.onJoinChannelSuccess = function(channel,  uid, elapsed){
                cc.log("[js] onJoinChannelSuccess, channel:%s,uid :%d, elapsed : %d !", channel, uid, elapsed);    
              
                self.addTips(" Join Channel Successfully !");
                // self.jSuccessNotify(channel,  uid, elapsed);
            };

            this.agoraAudioInst.onLeaveChannel = function (totalDuration, txBytes, rxBytes,txKBitRate,rxKBitRate,txAudioKBitRate,rxAudioKBitRate,txVideoKBitRate,rxVideoKBitRate,users,cpuTotalUsage,cpuAppUsage){
                cc.log("[js]onLeaveChannel, totalDuration:%s,utxBytes :%d, rxBytes : %d !", totalDuration, txBytes, rxBytes);
                // self.addTips(" Leave Channel Successfully !");
                // self.lSuccessNotify(totalDuration, txBytes, rxBytes,txKBitRate);
            };

            this.agoraAudioInst.onUserMuteAudio = function (userId,  muted){
                cc.log("[js]onUserMuteAudio, userId:%d, muted :%d !", userId, muted);
            };

            this.agoraAudioInst.onAudioVolumeIndication = function (speakers,  speakerNumber, totalVolume){
                cc.log("[js]onAudioVolumeIndication, speakerNumber:%d, totalVolume :%d !", speakerNumber, totalVolume);
                if (speakerNumber == 0) {
                    cc.log("[js] callback of Remote Speakers"); 
                }

                for (var i = 0;i <speakerNumber; i++) {
                    if (speakers[i].uid == 0 && speakerNumber == 1) {
                        cc.log("[js] Local Speaker[%d], uid:%d, volume : %d", i, speakers[i].uid, speakers[i].volume);
                        return;
                    }else{
                        cc.log("[js] Remote Speaker[%d], uid:%d, volume : %d", i, speakers[i].uid, speakers[i].volume);
                    }
                }
            };
        }
    },

    
    joinChannel:function(token, channelId, info, uid){

        if(this.agoraAudioInst == null){
            cc.log("agoraAudioInst  is null !");
            return false;
        }

        if(this._remoteVideoSprite == null){
            this._remoteVideoSprite =  new Map(); 
        };  
    
        var errCode = this.agoraAudioInst.joinChannel(g_roomName, uid, videoEnabled, videoMode, info);
        if( 0  == errCode){
            cc.log("logIn Successfully !\n");
        }else {
            cc.log("logIn Failed, errCode：%d\n",  errCode);
        }
    },
                             
    leaveChannel:function(){
        var errCode = this.agoraAudio.leaveChannel();
        if( 0 == errCode){
            cc.log("leaveChannel is called Successfully !\n");
        }else {
            cc.log("leaveChannel is called Failed, errCode：%d\n",  errCode);
        }
    },
};

module.exports = agoraAudio_module;
