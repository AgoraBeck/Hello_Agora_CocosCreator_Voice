//
//  jsb_agoraAudio.cpp
//
//  Created by on 18/3/3
//
//

#include "jsb_agoraAudio.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "scripting/js-bindings/manual/jsb_conversions.hpp"
#include "scripting/js-bindings/manual/jsb_global.h"
#include "cocos2d.h"
#include "platform/CCApplication.h"
#include "base/CCScheduler.h"

#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>

#include <string>
#include <vector>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define AGORA_CALL __cdecl
#if defined(AGORARTC_EXPORT)
#define AGORA_API extern "C" __declspec(dllexport)
#else
#define AGORA_API extern "C" __declspec(dllimport)
#endif
#elif defined(__APPLE__)
#define AGORA_API __attribute__((visibility("default"))) extern "C"
#define AGORA_CALL
#elif defined(__ANDROID__) || defined(__linux__) || defined(__linux)
#define AGORA_API extern "C" __attribute__((visibility("default")))
#define AGORA_CALL
#else
#define AGORA_API extern "C"
#define AGORA_CALL
#endif

#define   MAX_NUM  17
#if defined(__APPLE__)
     #include <AgoraAudioKit/IAgoraRtcEngine.h>
#elif defined(__ANDROID__)
    #include "IAgoraRtcEngine.h"
    // #include "../../../../../AgoraAuidoSDK/libs/Android/include/IAgoraRtcEngine.h"
    #include <android/log.h>
    #define LOG_TAG "android-debug"
    #define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#else

#endif



#include <string.h>

using namespace cocos2d;
using namespace agora::rtc;

class CAgoraAudioJsWrapper;
CAgoraAudioJsWrapper* g_SingleInstance;

se::Object *js_cocos2dx_agoraAudio_prototype = nullptr;
se::Class *js_cocos2dx_agoraAudio_class = nullptr;

class CAgoraAudioJsWrapper:public IRtcEngineEventHandler {
public:
//    static CAgoraAudioJsWrapper* getInstance();
    CAgoraAudioJsWrapper();
    ~CAgoraAudioJsWrapper();
    

public:
    se::Object* _refObj;

    /**
    * when join channel success, the function will be called
    * @param [in] channel
    *        the channel name you have joined
    * @param [in] userId
    *        the userId of you in this channel
    * @param [in] elapsed
    *        the time elapsed in ms from the joinChannel been called to joining completed
    */
    virtual void onJoinChannelSuccess(const char* channel, uid_t userId, int elapsed);

    /**
    * when join channel success, the function will be called
    * @param [in] channel
    *        the channel name you have joined
    * @param [in] userId
    *        the userId of you in this channel
    * @param [in] elapsed
    *        the time elapsed in ms elapsed
    */
    virtual void onRejoinChannelSuccess(const char* channel, uid_t userId, int elapsed);
    /**
    * when warning message coming, the function will be called
    * @param [in] warn
    *        warning code
    * @param [in] msg
    *        the warning message
    */
    virtual void onWarning(int warn, const char* msg);

    /**
    * when error message come, the function will be called
    * @param [in] err
    *        error code
    * @param [in] msg
    *        the error message
    */
    virtual void onError(int err, const char* msg);

    /**
    * when audio quality message come, the function will be called
    * @param [in] userId
    *        the userId of the peer
    * @param [in] quality
    *        the quality of the remote user, see QUALITY_TYPE for value definition
    * @param [in] delay
    *        the average time of the audio packages delayed
    * @param [in] lost
    *        the rate of the audio packages lost
    */
    virtual void onAudioQuality(uid_t userId, int quality, unsigned short delay, unsigned short lost);

    /**
    * when the audio volume information come, the function will be called
    * @param [in] speakers
    *        the array of the speakers' audio volume information
    * @param [in] speakerNumber
    *        the count of speakers in this array
    * @param [in] totalVolume
    *        the total volume of all users
    */
    virtual void onAudioVolumeIndication(const AudioVolumeInfo* speakers, unsigned int speakerNumber, int totalVolume);

    /**
    * when the audio volume information come, the function will be called
    * @param [in] stats
    *        the statistics of the call
    */
    virtual void onLeaveChannel(const RtcStats& stats);

    /**
    * when the information of the RTC engine stats come, the function will be called
    * @param [in] stats
    *        the RTC engine stats
    */
    virtual void onRtcStats(const RtcStats& stats);

    /**
    * when the audio device state changed(plugged or removed), the function will be called
    * @param [in] deviceId
    *        the ID of the state changed audio device
    * @param [in] deviceType
    *        the type of the audio device(playout device or record device)
    * @param [in] deviceState
    *        the device is been removed or added
    */
    virtual void onAudioDeviceStateChanged(const char* deviceId, int deviceType, int deviceState);

    /**
     * When audio mixing file playback finished, this function will be called
     */
    virtual void onAudioMixingFinished();

    /**
     * When far-end rhythm begins/ends, these functions will be called
     */
    virtual void onRemoteAudioMixingBegin();
    
    virtual void onRemoteAudioMixingEnd();
    /**
    * When audio effect playback finished, this function will be called
    */
    virtual void onAudioEffectFinished(int soundId);

    /**
    * report the network quality
	* @param [in] userId
	*        the userId of the remote user
	* @param [in] txQuality
    *        the score of the send network quality 0~5 the higher the better
	* @param [in] rxQuality
	*        the score of the recv network quality 0~5 the higher the better
	*/
    virtual void onNetworkQuality(uid_t userId, int txQuality, int rxQuality);

    /**
    * report the last-mile test network quality
    * @param [in] quality
    *        the score of the network quality 0~5 the higher the better
    */
    virtual void onLastmileQuality(int quality);

    /**
    * when any other user joined in the same channel, the function will be called
    * @param [in] userId
    *        the userId of the remote user
    * @param [in] elapsed
    *        the time elapsed from remote used called joinChannel to joining completed in ms
    */
    virtual void onUserJoined(uid_t userId, int elapsed);

    /**
    * when user offline(exit channel or offline by accident), the function will be called
    * @param [in] userId
    *        the userId of the remote user
    */
    virtual void onUserOffline(uid_t userId, USER_OFFLINE_REASON_TYPE reason);

    /**
    * when remote user muted the audio stream, the function will be called
    * @param [in] userId
    *        the userId of the remote user
    * @param [in] muted
    *        true: the remote user muted the audio stream, false: the remote user unmuted the audio stream
    */
    virtual void onUserMuteAudio(uid_t userId, bool muted);

   
    /**
    * when api call executed completely, the function will be called
    * @param [in] api
    *        the api name
    * @param [in] err
    *        error code while 0 means OK
    */
    virtual void onApiCallExecuted(int err, const char* api, const char* result);

    /**
    * when the network can not worked well, the function will be called
    */
    virtual void onConnectionLost();

    /**
    * when local user disconnected by accident, the function will be called(then SDK will try to reconnect itself)
    */
    virtual void onConnectionInterrupted();
    /**
     * when local user is banned by the server, the function will be called
     */
    virtual void onConnectionBanned();
    
    virtual void onRefreshRecordingServiceStatus(int status);

    /**
    * when stream message received, the function will be called
    * @param [in] userId
    *        userId of the peer who sends the message
    * @param [in] streamId
    *        APP can create multiple streams for sending messages of different purposes
    * @param [in] data
    *        the message data
    * @param [in] length
    *        the message length, in bytes
    *        frame rate
    */
    virtual void onStreamMessage(uid_t userId, int streamId, const char* data, size_t length);

    virtual void onStreamMessageError(uid_t userId, int streamId, int code, int missed, int cached) ;
    virtual void onMediaEngineLoadSuccess();
    virtual void onMediaEngineStartCallSuccess();
    /**
    * when token is enabled, and specified token is invalid or expired, this function will be called.
    * APP should generate a new token and call renewToken() to refresh the token.
    * NOTE: to be compatible with previous version, ERR_TOKEN_EXPIRED and ERR_INVALID_TOKEN are also reported via onError() callback.
    * You should move renew of token logic into this callback.
    */
    virtual void onRequestToken();

    /**
    * when the first local audio frame generated, the function will be called
    * @param [in] elapsed
    *        the time elapsed from remote user called joinChannel in ms
    */
    virtual void onFirstLocalAudioFrame(int elapsed);

    /**
    * when the first remote audio frame arrived, the function will be called
    * @param [in] userId
    *        the userId of the remote user
    * @param [in] elapsed
    *        the time elapsed from remote user called joinChannel in ms
    */
    virtual void onFirstRemoteAudioFrame(uid_t userId, int elapsed);
    /** @param [in] userId
    *        the speaker userId who is talking in the channel
    */
    virtual void onActiveSpeaker(uid_t userId);

    /**
    * when client role is successfully changed, the function will be called
    */
    virtual void onClientRoleChanged(CLIENT_ROLE_TYPE oldRole, CLIENT_ROLE_TYPE newRole);

    virtual void onAudioDeviceVolumeChanged(MEDIA_DEVICE_TYPE deviceType, int volume, bool muted);

    virtual void onStreamPublished(const char *url, int error);

    virtual void onStreamUnpublished(const char *url);

    virtual void onTranscodingUpdated();

    virtual void onStreamInjectedStatus(const char* url, uid_t userId, int status);

    virtual void onAudioRoutingChanged(int routing);

    /**
     * This callback is triggered when receiving audio packet from remote user
     * It implies the network statistics of audio packet
     *
     * @param [in] uid
     *        the UID of the remote user
     * @param [in] delay
     *        the end to end delay(ms) from the remote user to local client
     * @param [in] lost
     *        the packet lost rate
     * @param [in] rxKBitRate
     *        the receive audio KBps from remote user
     */
    virtual void onRemoteAudioTransportStats(
        uid_t uid, unsigned short delay, unsigned short lost,
                                             unsigned short rxKBitRate);

    /**
     * This callback is triggered when receiving video packet from remote user
     * It implies the network statistics of video packet
     *
     * @param [in] uid
     *        the UID of the remote user
     * @param [in] delay
     *        the end to end delay(ms) from the remote user to local client
     * @param [in] lost
     *        the packet lost rate
     * @param [in] rxKBitRate
     *        the receive video KBps from remote user
     */
    // virtual void onRemoteVideoTransportStats(
    //     uid_t uid, unsigned short delay, unsigned short lost,
    //     unsigned short rxKBitRate) {
    //     (void)uid;
    //     (void)delay;
    //     (void)lost;
    //     (void)rxKBitRate;
    // }

	/**
     *   Notify application the state of microphone has changed.
     *   true: Microphone is enabled.
     *   false: Microphone is disabled.
    */
    virtual void onMicrophoneEnabled(bool enabled) ;    
};

CAgoraAudioJsWrapper::CAgoraAudioJsWrapper()
{
}

CAgoraAudioJsWrapper::~CAgoraAudioJsWrapper()
{
}

void CAgoraAudioJsWrapper::onJoinChannelSuccess(const char* channel, uid_t userId, int elapsed)
{
    CCLOG("[Agora]:onJoinChannelSuccess %s, %u, %d", channel, userId, elapsed);
    static char channelName[0x100];
    strcpy(channelName, channel);
    
    Application::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
        se::Value func;
        if (_refObj->getProperty("onJoinChannelSuccess", &func)) {
            se::ScriptEngine::getInstance()->clearException();
            se::AutoHandleScope hs;
            se::ValueArray args;
            args.push_back(se::Value(channelName));
            args.push_back(se::Value((uid_t)userId));
            args.push_back(se::Value((int)elapsed));

            func.toObject()->call(args, _refObj);
        }
    });
    
    return ;
}

void CAgoraAudioJsWrapper:: onLeaveChannel(const RtcStats& stats)
{
    CCLOG("[Agora]:onLeaveChannel %d, %d",  stats.txBytes, stats.rxBytes);
    
    Application::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
        se::Value func;
        if(_refObj->getProperty("onLeaveChannel", &func)){
            se::ScriptEngine::getInstance()->clearException();
            se::AutoHandleScope hs;
            
            se::ValueArray args;
//            args.push_back(se::Value((int)stats.totalDuration));
            args.push_back(se::Value((int)stats.txBytes));
            args.push_back(se::Value((int)stats.rxBytes));
            args.push_back(se::Value((int)stats.txKBitRate));
            args.push_back(se::Value((int)stats.rxKBitRate));
            args.push_back(se::Value((int)stats.txAudioKBitRate));
            args.push_back(se::Value((int)stats.rxAudioKBitRate));
            args.push_back(se::Value((int)stats.txVideoKBitRate));
            args.push_back(se::Value((int)stats.rxVideoKBitRate));
//            args.push_back(se::Value((int)stats.users));
            args.push_back(se::Value((double)stats.cpuTotalUsage));
            args.push_back(se::Value((double)stats.cpuAppUsage));
            
            func.toObject()->call(args, _refObj);
        }
    });
    
    return ;
}

void CAgoraAudioJsWrapper::onRejoinChannelSuccess(const char* channel, uid_t userId, int elapsed) {

    CCLOG("[Agora]:onRejoinChannelSuccess %s, %u, %d", channel, userId, elapsed);
    static char channelName[0x100];
    strcpy(channelName, channel);
    
    Application::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
        se::Value func;
        if (_refObj->getProperty("onRejoinChannelSuccess", &func)) {
            se::ScriptEngine::getInstance()->clearException();
            se::AutoHandleScope hs;
            se::ValueArray args;
            args.push_back(se::Value(channelName));
            args.push_back(se::Value((uid_t)userId));
            args.push_back(se::Value((int)elapsed));
            
            func.toObject()->call(args, _refObj);
        }
    });
    return ;

}

void CAgoraAudioJsWrapper::onWarning(int warn, const char* msg) {
    CCLOG("[Agora]:onWarning %d, %s", warn, msg);
    if (msg == nullptr) {
        return;
    }
    std::string strMsg = msg;
    
    Application::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
        se::Value func;
        if (_refObj->getProperty("onWarning", &func)) {
            se::ScriptEngine::getInstance()->clearException();
            se::AutoHandleScope hs;
            se::ValueArray args;
            args.push_back(se::Value((int)warn));
            args.push_back(se::Value(strMsg));
            func.toObject()->call(args, _refObj);
        }
    });
    return ;
}

void CAgoraAudioJsWrapper::onError(int err, const char* msg) {
    if (msg == nullptr) {
        return;
    }
    CCLOG("[Agora]:onError %d, %s", err, msg);
    std::string strMsg = msg;
    
    Application::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
        se::Value func;
        if (_refObj->getProperty("onError", &func)) {
            se::ScriptEngine::getInstance()->clearException();
            se::AutoHandleScope hs;
            se::ValueArray args;
            args.push_back(se::Value((int)err));
            args.push_back(se::Value(strMsg));
            func.toObject()->call(args, _refObj);
        }
    });
    
    return ;
}

void CAgoraAudioJsWrapper::onAudioQuality(uid_t userId, int quality, unsigned short delay, unsigned short lost) {
    CCLOG("[Agora]:onAudioQuality %u, %d", userId, quality);
    
    Application::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
        se::Value func;
        if (_refObj->getProperty("onAudioQuality", &func)) {
            se::ScriptEngine::getInstance()->clearException();
            se::AutoHandleScope hs;
            se::ValueArray args;
            
            args.push_back(se::Value((uid_t)userId));
            args.push_back(se::Value((int)quality));
            args.push_back(se::Value((int)delay));
            args.push_back(se::Value((int)lost));
            func.toObject()->call(args, _refObj);
        }
    });
}

void CAgoraAudioJsWrapper::onAudioVolumeIndication(const AudioVolumeInfo* speakers, unsigned int speakerNumber, int totalVolume) {
    CCLOG("[Agora]:onAudioVolumeIndication %d, %d", speakerNumber, totalVolume);
    if(!speakerNumber){
        return;
    }
    
    AudioVolumeInfo speakersArr[MAX_NUM] = {0,};
    memcpy(speakersArr, speakers, speakerNumber*sizeof(AudioVolumeInfo));
    
    Application::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
        se::Value func;
        if (_refObj->getProperty("onAudioVolumeIndication", &func)) {
            se::ScriptEngine::getInstance()->clearException();
            se::AutoHandleScope hs;
            se::ValueArray args;
            
            se::Value out = se::Value::Null;
            se::HandleObject arr(se::Object::createArrayObject(speakerNumber));
            for (uint32_t i = 0; i < speakerNumber; ++i){
                se::Value uidOut = se::Value::Null;
                uint32_to_seval(speakersArr[i].uid, &uidOut);
                
                se::Value volumeOut = se::Value::Null;
                uint8_to_seval(speakersArr[i].volume, &volumeOut);
                
                se::HandleObject obj(se::Object::createPlainObject());
                obj->setProperty("uid",uidOut);
                obj->setProperty("volume", volumeOut);
                out.setObject(obj);
                arr->setArrayElement(i, se::Value(out));
            }
            
            args.push_back(se::Value(arr));
            args.push_back(se::Value(speakerNumber));
            args.push_back(se::Value(totalVolume));

            func.toObject()->call(args, _refObj);
        }
    });
}

void CAgoraAudioJsWrapper::onRtcStats(const RtcStats& stats) {
    CCLOG("[Agora]:onRtcStats %d, %d",  stats.txBytes, stats.rxBytes);
    
    Application::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
        se::Value func;
        if(_refObj->getProperty("onRtcStats", &func)){
            se::ScriptEngine::getInstance()->clearException();
            se::AutoHandleScope hs;
            
            se::ValueArray args;
            args.push_back(se::Value((int)stats.txBytes));
            args.push_back(se::Value((int)stats.rxBytes));
            args.push_back(se::Value((int)stats.txKBitRate));
            args.push_back(se::Value((int)stats.rxKBitRate));
            args.push_back(se::Value((int)stats.txAudioKBitRate));
            args.push_back(se::Value((int)stats.rxAudioKBitRate));
            args.push_back(se::Value((int)stats.txVideoKBitRate));
            args.push_back(se::Value((int)stats.rxVideoKBitRate));
            args.push_back(se::Value((double)stats.cpuTotalUsage));
            args.push_back(se::Value((double)stats.cpuAppUsage));
            
            func.toObject()->call(args, _refObj);
        }
    });
}


void CAgoraAudioJsWrapper::onAudioDeviceStateChanged(const char* deviceId, int deviceType, int deviceState) {
    CCLOG("[Agora]:onAudioDeviceStateChanged %s, %d", deviceId, deviceType);
    if (deviceId == nullptr) {
        return;
    }
    std::string strDeviceId = deviceId;
    
    Application::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
        se::Value func;
        if (_refObj->getProperty("onAudioDeviceStateChanged", &func)) {
            se::ScriptEngine::getInstance()->clearException();
            se::AutoHandleScope hs;
            se::ValueArray args;
            args.push_back(se::Value((int)deviceType));
            args.push_back(se::Value((int)deviceState));
            args.push_back(se::Value(strDeviceId));
            func.toObject()->call(args, _refObj);
        }
    });
}


void CAgoraAudioJsWrapper::onAudioMixingFinished() {
    CCLOG("[Agora]:onAudioMixingFinished ");
    
    Application::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
        se::Value func;
        if (_refObj->getProperty("onAudioMixingFinished", &func)) {
            se::ScriptEngine::getInstance()->clearException();
            se::AutoHandleScope hs;
            se::ValueArray args;
            func.toObject()->call(args, _refObj);
        }
    });
}

void CAgoraAudioJsWrapper::onRemoteAudioMixingBegin() {
    CCLOG("[Agora]:onRemoteAudioMixingBegin ");
    
    Application::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
        se::Value func;
        if (_refObj->getProperty("onRemoteAudioMixingBegin", &func)) {
            se::ScriptEngine::getInstance()->clearException();
            se::AutoHandleScope hs;
            se::ValueArray args;
            func.toObject()->call(args, _refObj);
        }
    });
}

void CAgoraAudioJsWrapper::onRemoteAudioMixingEnd() {
    CCLOG("[Agora]:onRemoteAudioMixingEnd ");
    
    Application::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
        se::Value func;
        if (_refObj->getProperty("onRemoteAudioMixingEnd", &func)) {
            se::ScriptEngine::getInstance()->clearException();
            se::AutoHandleScope hs;
            se::ValueArray args;
            func.toObject()->call(args, _refObj);
        }
    });
}

void CAgoraAudioJsWrapper::onAudioEffectFinished(int soundId) {
    CCLOG("[Agora]:onAudioEffectFinished ");
    
    Application::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
        se::Value func;
        if (_refObj->getProperty("onAudioEffectFinished", &func)) {
            se::ScriptEngine::getInstance()->clearException();
            se::AutoHandleScope hs;
            se::ValueArray args;
            func.toObject()->call(args, _refObj);
        }
    });
}

void CAgoraAudioJsWrapper::onNetworkQuality(uid_t userId, int txQuality, int rxQuality) {
    CCLOG("[Agora]:onNetworkQuality %u, %d, %d", userId, txQuality,rxQuality);
    
    Application::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
        se::Value func;
        if (_refObj->getProperty("onNetworkQuality", &func)) {
            se::ScriptEngine::getInstance()->clearException();
            se::AutoHandleScope hs;
            se::ValueArray args;
            args.push_back(se::Value((uid_t)userId));
            args.push_back(se::Value((int)txQuality));
            args.push_back(se::Value((int)rxQuality));
            func.toObject()->call(args, _refObj);
        }
    });
}

void CAgoraAudioJsWrapper::onLastmileQuality(int quality) {
    CCLOG("[Agora]:onLastmileQuality %d", quality);
    
    Application::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
        se::Value func;
        if (_refObj->getProperty("onLastmileQuality", &func)) {
            se::ScriptEngine::getInstance()->clearException();
            se::AutoHandleScope hs;
            se::ValueArray args;
            
            args.push_back(se::Value((int)quality));
            func.toObject()->call(args, _refObj);
        }
    });
}

void CAgoraAudioJsWrapper::onUserJoined(uid_t userId, int elapsed) {
    CCLOG("[Agora]:onUserJoined %u", userId);
    
    Application::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
        se::Value func;
        if (_refObj->getProperty("onUserJoined", &func)) {
            se::ScriptEngine::getInstance()->clearException();
            se::AutoHandleScope hs;
            se::ValueArray args;
            args.push_back(se::Value((uid_t)userId));
            args.push_back(se::Value((int)elapsed));
            func.toObject()->call(args, _refObj);
        }
    });
}

void CAgoraAudioJsWrapper::onUserOffline(uid_t userId, USER_OFFLINE_REASON_TYPE reason) {
    CCLOG("[Agora]:onUserOffline %u", userId);
    
    Application::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
        se::Value func;
        if (_refObj->getProperty("onUserOffline", &func)) {
            se::ScriptEngine::getInstance()->clearException();
            se::AutoHandleScope hs;
            se::ValueArray args;
            args.push_back(se::Value((uid_t)userId));
            args.push_back(se::Value((int)reason));
            func.toObject()->call(args, _refObj);
        }
    });
}

void CAgoraAudioJsWrapper::onUserMuteAudio(uid_t userId, bool muted) {
    CCLOG("[Agora]:onUserMuteAudio %u, %d", userId, muted);
    
    Application::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
        se::Value func;
        if (_refObj->getProperty("onUserMuteAudio", &func)) {
            se::ScriptEngine::getInstance()->clearException();
            se::AutoHandleScope hs;
            se::ValueArray args;
        
            args.push_back(se::Value((uid_t)userId));
            args.push_back(se::Value(muted));
            func.toObject()->call(args, _refObj);
        }
    });
}

void CAgoraAudioJsWrapper::onApiCallExecuted(int err, const char* api, const char* result) {
    CCLOG("[Agora]:onApiCallExecuted : %d, %s, %s", err, api, result);
    
    std::string apiMsg = api;
    std::string resultMsg = result;
    
    Application::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
        se::Value func;
        if (_refObj->getProperty("onApiCallExecuted", &func)) {
            se::ScriptEngine::getInstance()->clearException();
            se::AutoHandleScope hs;
            se::ValueArray args;
            
            args.push_back(se::Value((int)err));
            args.push_back(se::Value(apiMsg));
            args.push_back(se::Value(resultMsg));
            
            func.toObject()->call(args, _refObj);
        }
    });
}

void CAgoraAudioJsWrapper::onConnectionLost() {
    CCLOG("[Agora]:onConnectionLost ");
    
    Application::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
        se::Value func;
        if (_refObj->getProperty("onConnectionLost", &func)) {
            se::ScriptEngine::getInstance()->clearException();
            se::AutoHandleScope hs;
            se::ValueArray args;
            func.toObject()->call(args, _refObj);
        }
    });
}


void CAgoraAudioJsWrapper::onConnectionInterrupted() {
    CCLOG("[Agora]:onConnectionInterrupted ");
    
    Application::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
        se::Value func;
        if (_refObj->getProperty("onConnectionInterrupted", &func)) {
            se::ScriptEngine::getInstance()->clearException();
            se::AutoHandleScope hs;
            se::ValueArray args;
            func.toObject()->call(args, _refObj);
        }
    });
}


void CAgoraAudioJsWrapper::onConnectionBanned() {
    CCLOG("[Agora]:onConnectionBanned ");
    
    Application::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
        se::Value func;
        if (_refObj->getProperty("onConnectionBanned", &func)) {
            se::ScriptEngine::getInstance()->clearException();
            se::AutoHandleScope hs;
            se::ValueArray args;
            func.toObject()->call(args, _refObj);
        }
    });
}

void CAgoraAudioJsWrapper::onRefreshRecordingServiceStatus(int status) {
    (void)status;
}

void CAgoraAudioJsWrapper::onStreamMessage(uid_t userId, int streamId, const char* data, size_t length) {
    (void)userId;
    (void)streamId;
    (void)data;
    (void)length;
}

void CAgoraAudioJsWrapper::onStreamMessageError(uid_t userId, int streamId, int code, int missed, int cached) {
    (void)userId;
    (void)streamId;
    (void)code;
    (void)missed;
    (void)cached;
}

void CAgoraAudioJsWrapper::onMediaEngineLoadSuccess() {}
void CAgoraAudioJsWrapper::onMediaEngineStartCallSuccess() {
}
/**
* when token is enabled, and specified token is invalid or expired, this function will be called.
* APP should generate a new token and call renewToken() to refresh the token.
* NOTE: to be compatible with previous version, ERR_TOKEN_EXPIRED and ERR_INVALID_TOKEN are also reported via onError() callback.
* You should move renew of token logic into this callback.
*/
void CAgoraAudioJsWrapper::onRequestToken() {
    CCLOG("[Agora]:onRequestToken ");
    
    Application::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
        se::Value func;
        if (_refObj->getProperty("onRequestToken", &func)) {
            se::ScriptEngine::getInstance()->clearException();
            se::AutoHandleScope hs;
            se::ValueArray args;
            func.toObject()->call(args, _refObj);
        }
    });
}


void CAgoraAudioJsWrapper::onFirstLocalAudioFrame(int elapsed) {

    CCLOG("[Agora]:onFirstLocalAudioFrame : %d", elapsed);

    Application::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
        se::Value func;
        if (_refObj->getProperty("onFirstLocalAudioFrame", &func)) {
            se::ScriptEngine::getInstance()->clearException();
            se::AutoHandleScope hs;
            se::ValueArray args;
            
            args.push_back(se::Value((int)elapsed));
       
            func.toObject()->call(args, _refObj);
        }
    });
    
}


void CAgoraAudioJsWrapper::onFirstRemoteAudioFrame(uid_t userId, int elapsed) {
    
    CCLOG("[Agora]:onFirstRemoteAudioFrame : %u, %d", userId, elapsed);
    
    Application::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
        se::Value func;
        if (_refObj->getProperty("onFirstRemoteAudioFrame", &func)) {
            se::ScriptEngine::getInstance()->clearException();
            se::AutoHandleScope hs;
            se::ValueArray args;
            
            args.push_back(se::Value((uid_t)userId));
            args.push_back(se::Value((int)elapsed));
            
            func.toObject()->call(args, _refObj);
        }
    });
}

void CAgoraAudioJsWrapper::onActiveSpeaker(uid_t userId) {
    CCLOG("[Agora]:onActiveSpeaker : %u", userId);
    
    Application::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
        se::Value func;
        if (_refObj->getProperty("onActiveSpeaker", &func)) {
            se::ScriptEngine::getInstance()->clearException();
            se::AutoHandleScope hs;
            se::ValueArray args;
            
            args.push_back(se::Value((uid_t)userId));
            
            func.toObject()->call(args, _refObj);
        }
    });
}


void CAgoraAudioJsWrapper::onClientRoleChanged(CLIENT_ROLE_TYPE oldRole, CLIENT_ROLE_TYPE newRole) {
    CCLOG("[Agora]:onClientRoleChanged : %d, %d", oldRole, newRole);
    
    Application::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
        se::Value func;
        if (_refObj->getProperty("onClientRoleChanged", &func)) {
            se::ScriptEngine::getInstance()->clearException();
            se::AutoHandleScope hs;
            se::ValueArray args;
            
            args.push_back(se::Value((int)oldRole));
            args.push_back(se::Value((int)newRole));
            
            func.toObject()->call(args, _refObj);
        }
    });
}

void CAgoraAudioJsWrapper::onAudioDeviceVolumeChanged(MEDIA_DEVICE_TYPE deviceType, int volume, bool muted) {
    CCLOG("[Agora]:onAudioDeviceVolumeChanged : %d, %d", deviceType, volume);
    
    Application::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
        se::Value func;
        if (_refObj->getProperty("onAudioDeviceVolumeChanged", &func)) {
            se::ScriptEngine::getInstance()->clearException();
            se::AutoHandleScope hs;
            se::ValueArray args;
            
            args.push_back(se::Value((int)deviceType));
            args.push_back(se::Value((int)volume));
            args.push_back(se::Value(muted));
            
            func.toObject()->call(args, _refObj);
        }
    });
}

void CAgoraAudioJsWrapper::onStreamPublished(const char *url, int error) {
    (void)url;
    (void)error;
}

void CAgoraAudioJsWrapper::onStreamUnpublished(const char *url) {
    (void)url;
}

void CAgoraAudioJsWrapper::onTranscodingUpdated() {}

void CAgoraAudioJsWrapper::onStreamInjectedStatus(const char* url, uid_t userId, int status) {
    (void)url;
    (void)userId;
    (void)status;
}

void CAgoraAudioJsWrapper::onAudioRoutingChanged(int routing) {
    CCLOG("[Agora]:onAudioRoutingChanged : %d", routing);
    
    Application::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
        se::Value func;
        if (_refObj->getProperty("onAudioRoutingChanged", &func)) {
            se::ScriptEngine::getInstance()->clearException();
            se::AutoHandleScope hs;
            se::ValueArray args;
            
            args.push_back(se::Value((int)routing));
            
            func.toObject()->call(args, _refObj);
        }
    });
}


void CAgoraAudioJsWrapper::onRemoteAudioTransportStats(
    uid_t uid, unsigned short delay, unsigned short lost,
    unsigned short rxKBitRate) {
        (void)uid;
        (void)delay;
        (void)lost;
        (void)rxKBitRate;
}



void CAgoraAudioJsWrapper::onMicrophoneEnabled(bool enabled) {
	(void)enabled;
}


#define AGORA_CHECK(error) { int code = error; if (code != 0) CCLOG("[Agora Error] %s", agora()->getErrorDescription(code)); }

static bool js_cocos2dx_extension_agoraAudio_initialize(se::State& s)
{
    CCLOG("[Agora] js_cocos2dx_extension_agoraAudio_initialize");
    IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_extension_agoraAudio_initialize : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 1) {
        std::string appId;
        ok &= seval_to_std_string(args[0], &appId);
        SE_PRECONDITION2(ok, false, "js_cocos2dx_extension_agoraAudio_initialize : Error processing arguments");

        RtcEngineContext ctx;
        ctx.appId = appId.c_str();
	    ctx.eventHandler = g_SingleInstance;
        int ret = cobj->initialize(ctx);

        ok &= int32_to_seval(ret, &s.rval());
        SE_PRECONDITION2(ok, false, "js_cocos2dx_extension_agoraAudio_initialize : Error processing arguments");
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 1);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_extension_agoraAudio_initialize)

static bool js_cocos2dx_extension_agoraAudio_joinChannel(se::State& s)
{
    CCLOG("[Agora] js_cocos2dx_extension_agoraAudio_joinChannel");
    IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_extension_agoraAudio_joinChannel : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 4) {
        std::string token;
        ok &= seval_to_std_string(args[0], &token);

        std::string channelId;
        ok &= seval_to_std_string(args[1], &channelId);
        
        std::string info;
        ok &= seval_to_std_string(args[2], &info);      

        uint32_t uid;
        ok &= seval_to_uint32(args[3], &uid);

        int ret = cobj->joinChannel(token.c_str(), channelId.c_str(), info.c_str(), uid);
        /* beck test，测试, 开始 */
        cobj->enableAudioVolumeIndication(500, 3);
        /* beck test，测试 */
        
        int32_to_seval(ret, &s.rval());
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 4);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_extension_agoraAudio_joinChannel)

static bool js_cocos2dx_extension_agoraAudio_leaveChannel(se::State& s)
{
    CCLOG("leaveChannel() !!!");
    IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_extension_agoraAudio_leaveChannel: Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 0) {
        int ret = cobj->leaveChannel();
        ok &= int32_to_seval(ret, &s.rval());
        SE_PRECONDITION2(ok, false, "js_cocos2dx_extension_agoraAudio_leaveChannel : Error processing arguments");
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 0);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_extension_agoraAudio_leaveChannel)


static bool js_cocos2dx_extension_agoraAudio_getVersion(se::State& s) {
    CCLOG("getVersion()  !!!");
    IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
    // CAgoraAudioJsWrapper* cobj = (CAgoraAudioJsWrapper*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_extension_agoraAudio_getVersion: Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 0){
        int build = 0;
        const char* buildver = cobj->getVersion(&build);
        std::string strTemp(buildver);
        
        ok &= std_string_to_seval(strTemp,&s.rval());
        
        SE_PRECONDITION2(ok, false, "js_cocos2dx_extension_agoraAudio_getVersion : Error processing arguments");
        return true;
    }
    
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 0);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_extension_agoraAudio_getVersion)

static bool js_cocos2dx_extension_agoraAudio_getErrorDescription(se::State& s) {
    CCLOG("getErrorDescription() !!!");
    IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_extension_agoraAudio_getVersion: Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 1)
    {
        int code = 0;
        ok &= seval_to_int32(args[0], &code);
        
        const char* description = cobj->getErrorDescription(code);
        std::string strTemp(description);
        
        ok &= std_string_to_seval(strTemp,&s.rval());
        SE_PRECONDITION2(ok, false, "js_cocos2dx_extension_agoraAudio_getErrorDescription : Error processing arguments");
        return true;
    }
    
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 1);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_extension_agoraAudio_getErrorDescription)

static bool js_cocos2dx_extension_agoraAudio_setLogFilter(se::State& s) {
    CCLOG("setLogFilter() !!!");
    IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_extension_agoraAudio_setLogFilter: Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 1)
    {
        int filter = 0;
        ok &= seval_to_int32(args[0], &filter);
    
        int ret = cobj->setLogFilter(filter);
        
        int32_to_seval(ret, &s.rval());
   
        SE_PRECONDITION2(ok, false, "js_cocos2dx_extension_agoraAudio_getErrorDescription : Error processing arguments");
        return true;
    }
    
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 1);
    return false;
    
}
SE_BIND_FUNC(js_cocos2dx_extension_agoraAudio_setLogFilter)

static bool js_cocos2dx_extension_agoraAudio_setLogFile(se::State& s) {
    CCLOG("setLogFilter() !!!");
    IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_extension_agoraAudio_setLogFilter: Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 1)
    {
        std::string strVal;
        ok &= seval_to_std_string(args[0], &strVal);
        
        int ret = cobj->setLogFile(strVal.c_str());
        
        int32_to_seval(ret, &s.rval());
        
        SE_PRECONDITION2(ok, false, "js_cocos2dx_extension_agoraAudio_getErrorDescription : Error processing arguments");
        return true;
    }
    
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 1);
    return false;
    
}
SE_BIND_FUNC(js_cocos2dx_extension_agoraAudio_setLogFile)

static bool js_cocos2dx_extension_agoraAudio_renewToken(se::State& s)
{
    CCLOG("[Agora] js_cocos2dx_extension_agoraAudio_renewToken");
    IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_extension_agoraAudio_renewToken : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 1) {
        std::string token;
        ok &= seval_to_std_string(args[0], &token);
        
        int ret = cobj->renewToken(token.c_str());
   
        int32_to_seval(ret, &s.rval());
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 1);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_extension_agoraAudio_renewToken)

static bool js_cocos2dx_extension_agoraAudio_setChannelProfile(se::State& s) {
    CCLOG("setChannelProfile() !!!");
    IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_extension_agoraAudio_setChannelProfile: Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 1)
    {
        int profile = 0;
        ok &= seval_to_int32(args[0], &profile);
        
        int ret = cobj->setChannelProfile((agora::rtc::CHANNEL_PROFILE_TYPE)profile);
        
        int32_to_seval(ret, &s.rval());
        
        SE_PRECONDITION2(ok, false, "js_cocos2dx_extension_agoraAudio_setChannelProfile : Error processing arguments");
        return true;
    }
    
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 1);
    return false;
    
}
SE_BIND_FUNC(js_cocos2dx_extension_agoraAudio_setChannelProfile)


static bool js_cocos2dx_extension_agoraAudio_setClientRole(se::State& s) {
    CCLOG("setClientRole() !!!");
    IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_extension_agoraAudio_setChannelProfile: Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 1)
    {
        int role = 0;
        ok &= seval_to_int32(args[0], &role);
        
        int ret = cobj->setClientRole((agora::rtc::CLIENT_ROLE_TYPE)role);
        
        int32_to_seval(ret, &s.rval());
        
        SE_PRECONDITION2(ok, false, "js_cocos2dx_extension_agoraAudio_setClientRole : Error processing arguments");
        return true;
    }
    
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 1);
    return false;
    
}
SE_BIND_FUNC(js_cocos2dx_extension_agoraAudio_setClientRole)

static bool js_cocos2dx_extension_agoraAudio_enableLocalAudio(se::State& s) {
    CCLOG("enableLocalAudio() !!!");
    IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_extension_agoraAudio_enableLocalAudio: Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 1)
    {
        bool bVal;
        ok &= seval_to_boolean(args[0], &bVal);
        
        int  ret = cobj->enableLocalAudio(bVal);
        int32_to_seval(ret, &s.rval());
        
        SE_PRECONDITION2(ok, false, "js_cocos2dx_extension_agoraAudio_enableLocalAudio : Error processing arguments");
        return true;
    }
    
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 1);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_extension_agoraAudio_enableLocalAudio)

static bool js_cocos2dx_extension_agoraAudio_setAudioProfile(se::State& s) {
    CCLOG("setAudioProfile() !!!");
    IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_extension_agoraAudio_setAudioProfile: Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 2)
    {
        int profile = 0;
        ok &= seval_to_int32(args[0], &profile);
        
        int scenario = 0;
        ok &= seval_to_int32(args[1], &scenario);
        
        int ret = cobj->setAudioProfile((agora::rtc::AUDIO_PROFILE_TYPE)profile, (agora::rtc::AUDIO_SCENARIO_TYPE)scenario);
        
        int32_to_seval(ret, &s.rval());
        
        SE_PRECONDITION2(ok, false, "js_cocos2dx_extension_agoraAudio_setAudioProfile : Error processing arguments");
        return true;
    }
    
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 2);
    return false;
    
}
SE_BIND_FUNC(js_cocos2dx_extension_agoraAudio_setAudioProfile)


static bool js_cocos2dx_extension_agoraAudio_setDefaultMuteAllRemoteAudioStreams(se::State& s) {
    CCLOG("setDefaultMuteAllRemoteAudioStreams() !!!");
    IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_extension_agoraAudio_setDefaultMuteAllRemoteAudioStreams: Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 1)
    {
        bool bVal;
        ok &= seval_to_boolean(args[0], &bVal);
        
        int  ret = cobj->setDefaultMuteAllRemoteAudioStreams(bVal);
        int32_to_seval(ret, &s.rval());
        
        SE_PRECONDITION2(ok, false, "js_cocos2dx_extension_agoraAudio_setDefaultMuteAllRemoteAudioStreams : Error processing arguments");
        return true;
    }
    
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 1);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_extension_agoraAudio_setDefaultMuteAllRemoteAudioStreams)

static bool js_cocos2dx_extension_agoraAudio_setPlaybackDeviceVolume(se::State& s) {
    CCLOG("setPlaybackDeviceVolume() !!!");
    IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_extension_agoraAudio_setPlaybackDeviceVolume: Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 1)
    {
        int volume = 0;
        ok &= seval_to_int32(args[0], &volume);
        
        int ret = cobj->setPlaybackDeviceVolume(volume);
        
        int32_to_seval(ret, &s.rval());
        
        SE_PRECONDITION2(ok, false, "js_cocos2dx_extension_agoraAudio_setPlaybackDeviceVolume : Error processing arguments");
        return true;
    }
    
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 1);
    return false;
    
}
SE_BIND_FUNC(js_cocos2dx_extension_agoraAudio_setPlaybackDeviceVolume)

static bool js_cocos2dx_extension_agoraAudio_enableAudioVolumeIndication(se::State& s) {
    CCLOG("enableAudioVolumeIndication() !!!");
    IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_extension_agoraAudio_enableAudioVolumeIndication: Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 2)
    {
        int interval = 0;
        ok &= seval_to_int32(args[0], &interval);
        
        int smooth = 0;
        ok &= seval_to_int32(args[0], &smooth);
        
        int ret = cobj->enableAudioVolumeIndication(interval, smooth);
        
        int32_to_seval(ret, &s.rval());
        
        SE_PRECONDITION2(ok, false, "js_cocos2dx_extension_agoraAudio_enableAudioVolumeIndication : Error processing arguments");
        return true;
    }
    
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 2);
    return false;
    
}
SE_BIND_FUNC(js_cocos2dx_extension_agoraAudio_enableAudioVolumeIndication)

static bool js_cocos2dx_extension_agoraAudio_startAudioRecording(se::State& s)
{
    CCLOG("[Agora] startAudioRecording() ");
    IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_extension_agoraAudio_startAudioRecording : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 2) {
        std::string filePath;
        ok &= seval_to_std_string(args[0], &filePath);
    
        uint32_t quality;
        ok &= seval_to_uint32(args[1], &quality);
        
        int ret = cobj->startAudioRecording(filePath.c_str(), (agora::rtc::AUDIO_RECORDING_QUALITY_TYPE)quality);

        int32_to_seval(ret, &s.rval());
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 2);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_extension_agoraAudio_startAudioRecording)

static bool js_cocos2dx_extension_agoraAudio_stopAudioRecording(se::State& s)
{
    CCLOG("stopAudioRecording() !!!");
    IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_extension_agoraAudio_stopAudioRecording: Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 0) {
        int ret = cobj->stopAudioRecording();
        ok &= int32_to_seval(ret, &s.rval());
        SE_PRECONDITION2(ok, false, "js_cocos2dx_extension_agoraAudio_stopAudioRecording : Error processing arguments");
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 0);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_extension_agoraAudio_stopAudioRecording)

static bool js_cocos2dx_extension_agoraAudio_startAudioMixing(se::State& s)
{
    CCLOG("[Agora] startAudioMixing() ");
    IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_extension_agoraAudio_startAudioMixing : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 4) {
        std::string filePath;
        ok &= seval_to_std_string(args[0], &filePath);
        
        bool loopback = true;
        ok &= seval_to_boolean(args[1], &loopback);
        
        bool replace = true;
        ok &= seval_to_boolean(args[2], &replace);
        
        int cycle;
        ok &= seval_to_int32(args[3], &cycle);
        
        int ret = cobj->startAudioMixing(filePath.c_str(), loopback, replace, cycle);
        
        int32_to_seval(ret, &s.rval());
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 4);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_extension_agoraAudio_startAudioMixing)

static bool js_cocos2dx_extension_agoraAudio_stopAudioMixing(se::State& s)
{
    CCLOG("stopAudioMixing() !!!");
    IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_extension_agoraAudio_stopAudioMixing: Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 0) {
        int ret = cobj->stopAudioMixing();
        ok &= int32_to_seval(ret, &s.rval());
        SE_PRECONDITION2(ok, false, "js_cocos2dx_extension_agoraAudio_stopAudioMixing : Error processing arguments");
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 0);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_extension_agoraAudio_stopAudioMixing)

static bool js_cocos2dx_extension_agoraAudio_pauseAudioMixing(se::State& s)
{
    CCLOG("pauseAudioMixing() !!!");
    IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_extension_agoraAudio_pauseAudioMixing: Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 0) {
        int ret = cobj->pauseAudioMixing();
        ok &= int32_to_seval(ret, &s.rval());
        SE_PRECONDITION2(ok, false, "js_cocos2dx_extension_agoraAudio_pauseAudioMixing : Error processing arguments");
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 0);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_extension_agoraAudio_pauseAudioMixing)

static bool js_cocos2dx_extension_agoraAudio_resumeAudioMixing(se::State& s)
{
    CCLOG("resumeAudioMixing() !!!");
    IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_extension_agoraAudio_resumeAudioMixing: Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 0) {
        int ret = cobj->resumeAudioMixing();
        ok &= int32_to_seval(ret, &s.rval());
        SE_PRECONDITION2(ok, false, "js_cocos2dx_extension_agoraAudio_resumeAudioMixing : Error processing arguments");
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 0);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_extension_agoraAudio_resumeAudioMixing)

static bool js_cocos2dx_extension_agoraAudio_adjustAudioMixingVolume(se::State& s)
{
    CCLOG("[Agora] adjustAudioMixingVolume() ");
    IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_extension_agoraAudio_adjustAudioMixingVolume : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 1) {
        int volume;
        ok &= seval_to_int32(args[0], &volume);
        
        int ret = cobj->adjustAudioMixingVolume(volume);
        
        int32_to_seval(ret, &s.rval());
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 1);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_extension_agoraAudio_adjustAudioMixingVolume)

static bool js_cocos2dx_extension_agoraAudio_getAudioMixingDuration(se::State& s)
{
    CCLOG("getAudioMixingDuration() !!!");
    IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_extension_agoraAudio_getAudioMixingDuration: Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 0) {
        int ret = cobj->getAudioMixingDuration();
        ok &= int32_to_seval(ret, &s.rval());
        SE_PRECONDITION2(ok, false, "js_cocos2dx_extension_agoraAudio_getAudioMixingDuration : Error processing arguments");
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 0);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_extension_agoraAudio_getAudioMixingDuration)

static bool js_cocos2dx_extension_agoraAudio_getAudioMixingCurrentPosition(se::State& s)
{
    CCLOG("getAudioMixingCurrentPosition() !!!");
    IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_extension_agoraAudio_getAudioMixingCurrentPosition: Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 0) {
        int ret = cobj->getAudioMixingCurrentPosition();
        ok &= int32_to_seval(ret, &s.rval());
        SE_PRECONDITION2(ok, false, "js_cocos2dx_extension_agoraAudio_getAudioMixingCurrentPosition : Error processing arguments");
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 0);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_extension_agoraAudio_getAudioMixingCurrentPosition)

static bool js_cocos2dx_extension_agoraAudio_setAudioMixingPosition(se::State& s)
{
    CCLOG("[Agora] setAudioMixingPosition() ");
    IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_extension_agoraAudio_setAudioMixingPosition : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 1) {
        int pos;
        ok &= seval_to_int32(args[0], &pos);
        
        int ret = cobj->setAudioMixingPosition(pos);
        
        int32_to_seval(ret, &s.rval());
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 1);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_extension_agoraAudio_setAudioMixingPosition)

static bool js_cocos2dx_extension_agoraAudio_getEffectsVolume(se::State& s)
{
    CCLOG("[Agora] getEffectsVolume() ");
    IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_extension_agoraAudio_getEffectsVolume : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 0) {
        int ret = cobj->getEffectsVolume();
        
        int32_to_seval(ret, &s.rval());
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 0);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_extension_agoraAudio_getEffectsVolume)

static bool js_cocos2dx_extension_agoraAudio_setEffectsVolume(se::State& s)
{
    CCLOG("[Agora] setEffectsVolume() ");
    IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_extension_agoraAudio_setEffectsVolume : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 1) {
        int volume;
        ok &= seval_to_int32(args[0], &volume);
        
        int ret = cobj->setEffectsVolume(volume);
        
        int32_to_seval(ret, &s.rval());
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 1);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_extension_agoraAudio_setEffectsVolume)

static bool js_cocos2dx_extension_agoraAudio_setVolumeOfEffect(se::State& s)
{
    CCLOG("[Agora] setVolumeOfEffect() ");
    IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_extension_agoraAudio_setVolumeOfEffect : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 2) {
        int soundId;
        ok &= seval_to_int32(args[0], &soundId);
        
        int volume;
        ok &= seval_to_int32(args[1], &volume);
        
        int ret = cobj->setVolumeOfEffect(soundId, volume);
        
        int32_to_seval(ret, &s.rval());
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 2);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_extension_agoraAudio_setVolumeOfEffect)


static bool js_cocos2dx_extension_agoraAudio_playEffect(se::State& s)
{
    CCLOG("[Agora] playEffect() ");
    IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_extension_agoraAudio_playEffect : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 6 ) {
        int soundId = 0;
        ok &= seval_to_int32(args[0], &soundId);
        
        std::string filePath;
        ok &= seval_to_std_string(args[1], &filePath);
        
        int loopCount = 0;
        ok &= seval_to_int32(args[2], &loopCount);
        
        double pitch;
        ok &= seval_to_double(args[3], &pitch);
        
        double pan;
        ok &= seval_to_double(args[4], &pan);
        
        int gain = 0;
        ok &= seval_to_int32(args[5], &gain);
        
        int ret = cobj->playEffect(soundId, filePath.c_str(), loopCount, pitch, pan, gain);
        
        int32_to_seval(ret, &s.rval());
        return true;
       
    }else if(argc == 7){
        
        int soundId = 0;
        ok &= seval_to_int32(args[0], &soundId);
        
        std::string filePath;
        ok &= seval_to_std_string(args[1], &filePath);
        
        int loopCount = 0;
        ok &= seval_to_int32(args[2], &loopCount);
        
        double pitch;
        ok &= seval_to_double(args[3], &pitch);
        
        double pan;
        ok &= seval_to_double(args[4], &pan);
        
        int gain = 0;
        ok &= seval_to_int32(args[5], &gain);
        
        bool publish = false;
        ok &= seval_to_boolean(args[6], &publish);
        
        int ret = cobj->playEffect(soundId, filePath.c_str(), loopCount, pitch, pan, gain, publish);
        
        int32_to_seval(ret, &s.rval());
        return true;

    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d ", (int)argc, 6);
    
    return false;
}
SE_BIND_FUNC(js_cocos2dx_extension_agoraAudio_playEffect)


static bool js_cocos2dx_extension_agoraAudio_stopEffect(se::State& s)
{
    CCLOG("[Agora] stopEffect() ");
    IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_extension_agoraAudio_stopEffect : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 1) {
        int soundId = 0;
        ok &= seval_to_int32(args[0], &soundId);
        
        int ret = cobj->stopEffect(soundId);
        
        int32_to_seval(ret, &s.rval());
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 1);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_extension_agoraAudio_stopEffect)


static bool js_cocos2dx_extension_agoraAudio_stopAllEffects(se::State& s)
{
    CCLOG("[Agora] stopAllEffects() ");
    IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_extension_agoraAudio_stopAllEffects : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 0) {
        int ret = cobj->stopAllEffects();
        
        int32_to_seval(ret, &s.rval());
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 0);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_extension_agoraAudio_stopAllEffects)


static bool js_cocos2dx_extension_agoraAudio_preloadEffect(se::State& s)
{
    CCLOG("[Agora] preloadEffect() ");
    IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_extension_agoraAudio_preloadEffect : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 2) {
        int soundId = 0;
        ok &= seval_to_int32(args[0], &soundId);
        
        std::string filePath;
        ok &= seval_to_std_string(args[1], &filePath);
        
        int ret = cobj->preloadEffect(soundId, filePath.c_str());
        
        int32_to_seval(ret, &s.rval());
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 2);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_extension_agoraAudio_preloadEffect)

static bool js_cocos2dx_extension_agoraAudio_unloadEffect(se::State& s)
{
    CCLOG("[Agora] unloadEffect() ");
    IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_extension_agoraAudio_unloadEffect : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 1) {
        int soundId = 0;
        ok &= seval_to_int32(args[0], &soundId);
        
        int ret = cobj->unloadEffect(soundId);
        
        int32_to_seval(ret, &s.rval());
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 1);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_extension_agoraAudio_unloadEffect)

static bool js_cocos2dx_extension_agoraAudio_pauseEffect(se::State& s)
{
    CCLOG("[Agora] pauseEffect() ");
    IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_extension_agoraAudio_pauseEffect : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 1) {
        int soundId = 0;
        ok &= seval_to_int32(args[0], &soundId);
        
        int ret = cobj->pauseEffect(soundId);
        
        int32_to_seval(ret, &s.rval());
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 1);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_extension_agoraAudio_pauseEffect)

static bool js_cocos2dx_extension_agoraAudio_pauseAllEffects(se::State& s)
{
    CCLOG("[Agora] pauseAllEffects() ");
    IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_extension_agoraAudio_pauseAllEffects : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 0) {
        int ret = cobj->pauseAllEffects();
        ok &= int32_to_seval(ret, &s.rval());
        SE_PRECONDITION2(ok, false, "js_cocos2dx_extension_agoraAudio_pauseAllEffects : Error processing arguments");
        return true;
    }
    
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 0);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_extension_agoraAudio_pauseAllEffects)

static bool js_cocos2dx_extension_agoraAudio_resumeEffect(se::State& s)
{
    CCLOG("[Agora] resumeEffect() ");
    IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_extension_agoraAudio_resumeEffect : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 1) {
        int soundId = 0;
        ok &= seval_to_int32(args[0], &soundId);
        
        int ret = cobj->resumeEffect(soundId);
        
        int32_to_seval(ret, &s.rval());
        SE_PRECONDITION2(ok, false, "js_cocos2dx_extension_agoraAudio_resumeEffect : Error processing arguments");
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 1);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_extension_agoraAudio_resumeEffect)

static bool js_cocos2dx_extension_agoraAudio_resumeAllEffects(se::State& s)
{
    CCLOG("[Agora] resumeAllEffects() ");
    IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_extension_agoraAudio_resumeAllEffects : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 0) {
        int ret = cobj->resumeAllEffects();
        
        int32_to_seval(ret, &s.rval());
        SE_PRECONDITION2(ok, false, "js_cocos2dx_extension_agoraAudio_resumeAllEffects : Error processing arguments");
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 0);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_extension_agoraAudio_resumeAllEffects)

static bool js_cocos2dx_extension_agoraAudio_setLocalVoicePitch(se::State& s)
{
    CCLOG("[Agora] setLocalVoicePitch() ");
    IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_extension_agoraAudio_setLocalVoicePitch : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 1) {
        double pitch = 0;
        ok &= seval_to_double(args[0], &pitch);
        
        int ret = cobj->setLocalVoicePitch(pitch);
        
        int32_to_seval(ret, &s.rval());
        SE_PRECONDITION2(ok, false, "js_cocos2dx_extension_agoraAudio_setLocalVoicePitch : Error processing arguments");
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 1);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_extension_agoraAudio_setLocalVoicePitch)

static bool js_cocos2dx_extension_agoraAudio_setRemoteVoicePosition(se::State& s)
{
    CCLOG("[Agora] setRemoteVoicePosition() ");
    IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_extension_agoraAudio_setRemoteVoicePosition : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 3) {
        int uid;
        ok &= seval_to_int32(args[0], &uid);
        
        double pan = 0;
        ok &= seval_to_double(args[1], &pan);
        
        double gain = 0;
        ok &= seval_to_double(args[2], &gain);
        
        int ret = cobj->setRemoteVoicePosition(uid, pan, gain);
        
        int32_to_seval(ret, &s.rval());
        SE_PRECONDITION2(ok, false, "js_cocos2dx_extension_agoraAudio_setRemoteVoicePosition : Error processing arguments");
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 3);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_extension_agoraAudio_setRemoteVoicePosition)

static bool js_cocos2dx_extension_agoraAudio_setVoiceOnlyMode(se::State& s) {
    CCLOG("setVoiceOnlyMode() !!!");
    IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_extension_agoraAudio_setVoiceOnlyMode: Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 1)
    {
        bool enable;
        ok &= seval_to_boolean(args[0], &enable);
        int  ret = cobj->setVoiceOnlyMode(enable);
        int32_to_seval(ret, &s.rval());
        
        SE_PRECONDITION2(ok, false, "js_cocos2dx_extension_agoraAudio_setVoiceOnlyMode : Error processing arguments");
        return true;
    }
    
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 1);
    return false;
    
}
SE_BIND_FUNC(js_cocos2dx_extension_agoraAudio_setVoiceOnlyMode)


static bool js_cocos2dx_extension_agoraAudio_setLocalVoiceEqualization(se::State& s)
{
    CCLOG("[Agora] setLocalVoiceEqualization() ");
    IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_extension_agoraAudio_setLocalVoiceEqualization : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 2) {
        int bandFrequency = 0;
        ok &= seval_to_int32(args[0], &bandFrequency);
        
        int bandGain = 0;
        ok &= seval_to_int32(args[1], &bandGain);
        
        int ret = cobj->setLocalVoiceEqualization((agora::rtc::AUDIO_EQUALIZATION_BAND_FREQUENCY)bandFrequency, bandGain);
        
        int32_to_seval(ret, &s.rval());
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 2);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_extension_agoraAudio_setLocalVoiceEqualization)

static bool js_cocos2dx_extension_agoraAudio_setLocalVoiceReverb(se::State& s)
{
    CCLOG("[Agora] setLocalVoiceReverb() ");
    IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_extension_agoraAudio_setLocalVoiceReverb : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 2) {
        int reverbKey = 0;
        ok &= seval_to_int32(args[0], &reverbKey);
        
        int value = 0;
        ok &= seval_to_int32(args[1], &value);
        
        int ret = cobj->setLocalVoiceReverb((agora::rtc::AUDIO_REVERB_TYPE)reverbKey, value);
        
        int32_to_seval(ret, &s.rval());
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 2);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_extension_agoraAudio_setLocalVoiceReverb)

static bool js_cocos2dx_extension_agoraAudio_pauseAudio(se::State& s)
{
    CCLOG("[Agora] pauseAudio() ");
    IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_extension_agoraAudio_pauseAudio : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 0) {
        int ret = cobj->pauseAudio();
        
        int32_to_seval(ret, &s.rval());
        SE_PRECONDITION2(ok, false, "js_cocos2dx_extension_agoraAudio_pauseAudio : Error processing arguments");
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 0);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_extension_agoraAudio_pauseAudio)

static bool js_cocos2dx_extension_agoraAudio_resumeAudio(se::State& s)
{
    CCLOG("[Agora] resumeAudio() ");
    IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_extension_agoraAudio_resumeAudio : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 0) {
        int ret = cobj->resumeAudio();
        
        int32_to_seval(ret, &s.rval());
        SE_PRECONDITION2(ok, false, "js_cocos2dx_extension_agoraAudio_resumeAudio : Error processing arguments");
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 0);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_extension_agoraAudio_resumeAudio)

static bool js_cocos2dx_extension_agoraAudio_adjustRecordingSignalVolume(se::State& s)
{
    CCLOG("[Agora] adjustRecordingSignalVolume() ");
    IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_extension_agoraAudio_adjustRecordingSignalVolume : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 1) {
        int volume = 0;
        ok &= seval_to_int32(args[0], &volume);
        
        int ret = cobj->adjustRecordingSignalVolume(volume);
        
        int32_to_seval(ret, &s.rval());
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 1);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_extension_agoraAudio_adjustRecordingSignalVolume)

static bool js_cocos2dx_extension_agoraAudio_adjustPlaybackSignalVolume(se::State& s)
{
    CCLOG("[Agora] adjustPlaybackSignalVolume() ");
    IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_extension_agoraAudio_adjustPlaybackSignalVolume : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 1) {
        int volume = 0;
        ok &= seval_to_int32(args[0], &volume);
        
        int ret = cobj->adjustPlaybackSignalVolume(volume);
        
        int32_to_seval(ret, &s.rval());
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 1);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_extension_agoraAudio_adjustPlaybackSignalVolume)

static bool js_cocos2dx_extension_agoraAudio_setHighQualityAudioParameters(se::State& s) {
    CCLOG("setHighQualityAudioParameters() !!!");
    IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_extension_agoraAudio_setHighQualityAudioParameters: Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 3)
    {
        bool fullband;
        ok &= seval_to_boolean(args[0], &fullband);
        
        bool stereo;
        ok &= seval_to_boolean(args[0], &stereo);
        
        bool fullBitrate;
        ok &= seval_to_boolean(args[0], &fullBitrate);
        
        int  ret = cobj->setHighQualityAudioParameters(fullband, stereo, fullBitrate);
        int32_to_seval(ret, &s.rval());
        
        SE_PRECONDITION2(ok, false, "js_cocos2dx_extension_agoraAudio_setHighQualityAudioParameters : Error processing arguments");
        return true;
    }
    
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 3);
    return false;
    
}
SE_BIND_FUNC(js_cocos2dx_extension_agoraAudio_setHighQualityAudioParameters)

static bool js_cocos2dx_extension_agoraAudio_enableWebSdkInteroperability(se::State& s) {
    CCLOG("enableWebSdkInteroperability() !!!");
    IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_extension_agoraAudio_enableWebSdkInteroperability: Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 1)
    {
        bool enabled;
        ok &= seval_to_boolean(args[0], &enabled);
        
        int  ret = cobj->enableWebSdkInteroperability(enabled);
        int32_to_seval(ret, &s.rval());
        
        SE_PRECONDITION2(ok, false, "js_cocos2dx_extension_agoraAudio_enableWebSdkInteroperability : Error processing arguments");
        return true;
    }
    
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 1);
    return false;
    
}
SE_BIND_FUNC(js_cocos2dx_extension_agoraAudio_enableWebSdkInteroperability)

static bool js_cocos2dx_extension_agoraAudio_setInEarMonitoringVolume(se::State& s) {
    CCLOG("setInEarMonitoringVolume() !!!");
    IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_extension_agoraAudio_setInEarMonitoringVolume: Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 1)
    {
        int volume = 0;
        ok &= seval_to_int32(args[0], &volume);
        
        int  ret = cobj->setInEarMonitoringVolume(volume);
        int32_to_seval(ret, &s.rval());
        
        SE_PRECONDITION2(ok, false, "js_cocos2dx_extension_agoraAudio_setInEarMonitoringVolume : Error processing arguments");
        return true;
    }
    
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 1);
    return false;
    
}
SE_BIND_FUNC(js_cocos2dx_extension_agoraAudio_setInEarMonitoringVolume)

static bool js_cocos2dx_extension_agoraAudio_isSpeakerphoneEnabled(se::State& s) {
    CCLOG("isSpeakerphoneEnabled() !!!");
    IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_extension_agoraAudio_isSpeakerphoneEnabled: Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 0)
    {
        bool ret = cobj->isSpeakerphoneEnabled();
        boolean_to_seval(ret, &s.rval());
        
        SE_PRECONDITION2(ok, false, "js_cocos2dx_extension_agoraAudio_isSpeakerphoneEnabled : Error processing arguments");
        return true;
    }
    
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 0);
    return false;
    
}
SE_BIND_FUNC(js_cocos2dx_extension_agoraAudio_isSpeakerphoneEnabled)

static bool js_cocos2dx_extension_agoraAudio_setEncryptionMode(se::State& s)
{
    CCLOG("[Agora] setEncryptionMode");
    IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_extension_agoraAudio_setEncryptionMode : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 1) {
        std::string encryptionMode;
        ok &= seval_to_std_string(args[0], &encryptionMode);
        int ret = cobj->setEncryptionMode(encryptionMode.c_str());
        int32_to_seval(ret, &s.rval());
        SE_PRECONDITION2(ok, false, "js_cocos2dx_extension_agoraAudio_setEncryptionMode : Error processing arguments");
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 1);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_extension_agoraAudio_setEncryptionMode)

static bool js_cocos2dx_extension_agoraAudio_setEncryptionSecret(se::State& s)
{
    CCLOG("[Agora] setEncryptionSecret");
    IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_extension_agoraAudio_setEncryptionSecret : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 1) {
        std::string secret;
        ok &= seval_to_std_string(args[0], &secret);
        int ret = cobj->setEncryptionSecret(secret.c_str());
        int32_to_seval(ret, &s.rval());
        SE_PRECONDITION2(ok, false, "js_cocos2dx_extension_agoraAudio_setEncryptionSecret : Error processing arguments");
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 1);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_extension_agoraAudio_setEncryptionSecret)

static bool js_cocos2dx_extension_agoraAudio_enableAudio(se::State& s) {
    CCLOG("enableAudio() !!!");
    IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_extension_agoraAudio_enableAudio: Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 0)
    {
        int  ret = cobj->enableAudio();
        int32_to_seval(ret, &s.rval());
        
        SE_PRECONDITION2(ok, false, "js_cocos2dx_extension_agoraAudio_enableAudio : Error processing arguments");
        return true;
    }
    
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 0);
    return false;
    
}
SE_BIND_FUNC(js_cocos2dx_extension_agoraAudio_enableAudio)

static bool js_cocos2dx_extension_agoraAudio_disableAudio(se::State& s) {
    CCLOG("disableAudio() !!!");
    IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_extension_agoraAudio_disableAudio: Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 0)
    {
        int  ret = cobj->disableAudio();
        int32_to_seval(ret, &s.rval());
        
        SE_PRECONDITION2(ok, false, "js_cocos2dx_extension_agoraAudio_disableAudio : Error processing arguments");
        return true;
    }
    
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 0);
    return false;
    
}
SE_BIND_FUNC(js_cocos2dx_extension_agoraAudio_disableAudio)

static bool js_cocos2dx_extension_agoraAudio_muteLocalAudioStream(se::State& s) {
    CCLOG("muteLocalAudioStream() !!!");
    IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_extension_agoraAudio_muteLocalAudioStream: Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 1)
    {
        bool bVal;
        ok &= seval_to_boolean(args[0], &bVal);

        int  ret = cobj->muteLocalAudioStream(bVal);
        int32_to_seval(ret, &s.rval());
        
        SE_PRECONDITION2(ok, false, "js_cocos2dx_extension_agoraAudio_muteLocalAudioStream : Error processing arguments");
        return true;
    }
    
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 0);
    return false;
    
}
SE_BIND_FUNC(js_cocos2dx_extension_agoraAudio_muteLocalAudioStream)

static bool js_cocos2dx_extension_agoraAudio_muteAllRemoteAudioStreams(se::State& s) {
    CCLOG("muteLocalAudioStream() !!!");
    IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_extension_agoraAudio_muteAllRemoteAudioStreams: Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 1)
    {
        bool bVal;
        ok &= seval_to_boolean(args[0], &bVal);
        
        int  ret = cobj->muteAllRemoteAudioStreams(bVal);
        int32_to_seval(ret, &s.rval());
        
        SE_PRECONDITION2(ok, false, "js_cocos2dx_extension_agoraAudio_muteAllRemoteAudioStreams : Error processing arguments");
        return true;
    }
    
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 1);
    return false;
    
}
SE_BIND_FUNC(js_cocos2dx_extension_agoraAudio_muteAllRemoteAudioStreams)

static bool js_cocos2dx_extension_agoraAudio_muteRemoteAudioStream(se::State& s) {
    CCLOG("muteRemoteAudioStream() !!!");
    IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_extension_agoraAudio_muteRemoteAudioStream: Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 2)
    {
        uint32_t uid;
        ok &= seval_to_uint32(args[0], &uid);
        
        bool bVal;
        ok &= seval_to_boolean(args[1], &bVal);
        
        int  ret = cobj->muteRemoteAudioStream(uid, bVal);
        int32_to_seval(ret, &s.rval());
        
        SE_PRECONDITION2(ok, false, "js_cocos2dx_extension_agoraAudio_muteRemoteAudioStream : Error processing arguments");
        return true;
    }
    
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 2);
    return false;
    
}
SE_BIND_FUNC(js_cocos2dx_extension_agoraAudio_muteRemoteAudioStream)

static bool js_cocos2dx_extension_agoraAudio_setDefaultAudioRouteToSpeakerphone(se::State& s) {
    CCLOG("setDefaultAudioRouteToSpeakerphone() !!!");
    IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_extension_agoraAudio_setDefaultAudioRouteToSpeakerphone: Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 1)
    {
        bool bVal;
        ok &= seval_to_boolean(args[0], &bVal);
        
        int  ret = cobj->setDefaultAudioRouteToSpeakerphone(bVal);
        int32_to_seval(ret, &s.rval());
        
        SE_PRECONDITION2(ok, false, "js_cocos2dx_extension_agoraAudio_setDefaultAudioRouteToSpeakerphone : Error processing arguments");
        return true;
    }
    
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 1);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_extension_agoraAudio_setDefaultAudioRouteToSpeakerphone)

static bool js_cocos2dx_extension_agoraAudio_setEnableSpeakerphone(se::State& s) {
    CCLOG("setEnableSpeakerphone() !!!");
    IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_extension_agoraAudio_setEnableSpeakerphone: Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 1)
    {
        bool bVal;
        ok &= seval_to_boolean(args[0], &bVal);
        
        int  ret = cobj->setEnableSpeakerphone(bVal);
        int32_to_seval(ret, &s.rval());
        
        SE_PRECONDITION2(ok, false, "js_cocos2dx_extension_agoraAudio_setEnableSpeakerphone : Error processing arguments");
        return true;
    }
    
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 0);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_extension_agoraAudio_setEnableSpeakerphone)


static bool js_agoraAudio_finalize(se::State& s){
    CCLOGINFO("jsbindings: finalizing JS object %p (agoraAudio)", s.nativeThisObject());
    auto iter = se::NonRefNativePtrCreatedByCtorMap::find(s.nativeThisObject());
    if (iter != se::NonRefNativePtrCreatedByCtorMap::end())
    {
        se::NonRefNativePtrCreatedByCtorMap::erase(iter);
         IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
        //delete cobj;
		if(cobj){
			cobj->release();
        }
    }
    return true;
}
SE_BIND_FINALIZE_FUNC(js_agoraAudio_finalize)

static bool js_cocos2dx_extension_agoraAudio_constructor(se::State& s)
{
    CCLOG("[Agora] constructor");
//    const auto& args = s.args();
    
//    size_t argc = args.size();
    
    if (g_SingleInstance == NULL){
        g_SingleInstance = new CAgoraAudioJsWrapper();
    }
    
    se::Object* obj = s.thisObject();
    
    // link the native object with the javascript object
    g_SingleInstance->_refObj = obj;

    IRtcEngine *mAgoraEngine = (IRtcEngine *)createAgoraRtcEngine();
    if (obj) {
        obj->setPrivateData(mAgoraEngine);
        se::Value func;
        if (obj->getProperty("_ctor", &func)) {
            func.toObject()->call(se::EmptyValueArray, obj);
        }
    }
    
    return true;
}
SE_BIND_CTOR(js_cocos2dx_extension_agoraAudio_constructor, js_cocos2dx_agoraAudio_class, js_agoraAudio_finalize)


bool js_register_cocos2dx_extension_agoraAudio(se::Object* obj)
{
    CCLOG("[Agora] js_register_cocos2dx_extension_agoraAudio");
    
    auto cls = se::Class::create("agoraAudio", obj, nullptr, _SE(js_cocos2dx_extension_agoraAudio_constructor));
    
    cls->defineFunction("initialize", _SE(js_cocos2dx_extension_agoraAudio_initialize));
    cls->defineFunction("joinChannel", _SE(js_cocos2dx_extension_agoraAudio_joinChannel));
    cls->defineFunction("leaveChannel", _SE(js_cocos2dx_extension_agoraAudio_leaveChannel));
    cls->defineFunction("getVersion", _SE(js_cocos2dx_extension_agoraAudio_getVersion));
    cls->defineFunction("getErrorDescription", _SE(js_cocos2dx_extension_agoraAudio_getErrorDescription));
    cls->defineFunction("setLogFilter", _SE(js_cocos2dx_extension_agoraAudio_setLogFilter));
    cls->defineFunction("setLogFile", _SE(js_cocos2dx_extension_agoraAudio_setLogFile));
    cls->defineFunction("renewToken", _SE(js_cocos2dx_extension_agoraAudio_renewToken));
    cls->defineFunction("setChannelProfile", _SE(js_cocos2dx_extension_agoraAudio_setChannelProfile));
    cls->defineFunction("setClientRole", _SE(js_cocos2dx_extension_agoraAudio_setClientRole));

    cls->defineFunction("enableLocalAudio", _SE(js_cocos2dx_extension_agoraAudio_enableLocalAudio));
    cls->defineFunction("setDefaultMuteAllRemoteAudioStreams", _SE(js_cocos2dx_extension_agoraAudio_setDefaultMuteAllRemoteAudioStreams));
    
    cls->defineFunction("setInEarMonitoringVolume", _SE(js_cocos2dx_extension_agoraAudio_setInEarMonitoringVolume));
    cls->defineFunction("isSpeakerphoneEnabled", _SE(js_cocos2dx_extension_agoraAudio_isSpeakerphoneEnabled));
    cls->defineFunction("setEncryptionMode", _SE(js_cocos2dx_extension_agoraAudio_setEncryptionMode));
    cls->defineFunction("enableWebSdkInteroperability", _SE(js_cocos2dx_extension_agoraAudio_enableWebSdkInteroperability));
    cls->defineFunction("setHighQualityAudioParameters", _SE(js_cocos2dx_extension_agoraAudio_setHighQualityAudioParameters));
    cls->defineFunction("adjustPlaybackSignalVolume", _SE(js_cocos2dx_extension_agoraAudio_adjustPlaybackSignalVolume));
    cls->defineFunction("adjustRecordingSignalVolume", _SE(js_cocos2dx_extension_agoraAudio_adjustRecordingSignalVolume));
    cls->defineFunction("resumeAudio", _SE(js_cocos2dx_extension_agoraAudio_resumeAudio));
    cls->defineFunction("pauseAudio", _SE(js_cocos2dx_extension_agoraAudio_pauseAudio));
    cls->defineFunction("setLocalVoiceReverb", _SE(js_cocos2dx_extension_agoraAudio_setLocalVoiceReverb));
    cls->defineFunction("setLocalVoiceEqualization", _SE(js_cocos2dx_extension_agoraAudio_setLocalVoiceEqualization));
    cls->defineFunction("setVoiceOnlyMode", _SE(js_cocos2dx_extension_agoraAudio_setVoiceOnlyMode));
    cls->defineFunction("setRemoteVoicePosition", _SE(js_cocos2dx_extension_agoraAudio_setRemoteVoicePosition));
    cls->defineFunction("setLocalVoicePitch", _SE(js_cocos2dx_extension_agoraAudio_setLocalVoicePitch));
    cls->defineFunction("resumeAllEffects", _SE(js_cocos2dx_extension_agoraAudio_resumeAllEffects));
    
    cls->defineFunction("resumeEffect", _SE(js_cocos2dx_extension_agoraAudio_resumeEffect));
    cls->defineFunction("pauseAllEffects", _SE(js_cocos2dx_extension_agoraAudio_pauseAllEffects));
    cls->defineFunction("pauseEffect", _SE(js_cocos2dx_extension_agoraAudio_pauseEffect));
    cls->defineFunction("unloadEffect", _SE(js_cocos2dx_extension_agoraAudio_unloadEffect));
    cls->defineFunction("preloadEffect", _SE(js_cocos2dx_extension_agoraAudio_preloadEffect));
    cls->defineFunction("stopAllEffects", _SE(js_cocos2dx_extension_agoraAudio_stopAllEffects));
    cls->defineFunction("stopEffect", _SE(js_cocos2dx_extension_agoraAudio_stopEffect));
    cls->defineFunction("playEffect", _SE(js_cocos2dx_extension_agoraAudio_playEffect));
    cls->defineFunction("setVolumeOfEffect", _SE(js_cocos2dx_extension_agoraAudio_setVolumeOfEffect));
    cls->defineFunction("setEffectsVolume", _SE(js_cocos2dx_extension_agoraAudio_setEffectsVolume));
    cls->defineFunction("getEffectsVolume", _SE(js_cocos2dx_extension_agoraAudio_getEffectsVolume));
    cls->defineFunction("setAudioMixingPosition", _SE(js_cocos2dx_extension_agoraAudio_setAudioMixingPosition));
    
    cls->defineFunction("getAudioMixingCurrentPosition", _SE(js_cocos2dx_extension_agoraAudio_getAudioMixingCurrentPosition));
    cls->defineFunction("getAudioMixingDuration", _SE(js_cocos2dx_extension_agoraAudio_getAudioMixingDuration));
    cls->defineFunction("adjustAudioMixingVolume", _SE(js_cocos2dx_extension_agoraAudio_adjustAudioMixingVolume));
    cls->defineFunction("resumeAudioMixing", _SE(js_cocos2dx_extension_agoraAudio_resumeAudioMixing));
    cls->defineFunction("pauseAudioMixing", _SE(js_cocos2dx_extension_agoraAudio_pauseAudioMixing));
    cls->defineFunction("stopAudioMixing", _SE(js_cocos2dx_extension_agoraAudio_stopAudioMixing));
    cls->defineFunction("startAudioMixing", _SE(js_cocos2dx_extension_agoraAudio_startAudioMixing));
    cls->defineFunction("stopAudioRecording", _SE(js_cocos2dx_extension_agoraAudio_stopAudioRecording));
    cls->defineFunction("startAudioRecording", _SE(js_cocos2dx_extension_agoraAudio_startAudioRecording));
    cls->defineFunction("enableAudioVolumeIndication", _SE(js_cocos2dx_extension_agoraAudio_enableAudioVolumeIndication));
    cls->defineFunction("setPlaybackDeviceVolume", _SE(js_cocos2dx_extension_agoraAudio_setPlaybackDeviceVolume));
    
    cls->defineFunction("enableAudio", _SE(js_cocos2dx_extension_agoraAudio_enableAudio));
    cls->defineFunction("disableAudio", _SE(js_cocos2dx_extension_agoraAudio_disableAudio));
    cls->defineFunction("muteLocalAudioStream", _SE(js_cocos2dx_extension_agoraAudio_muteLocalAudioStream));
    cls->defineFunction("muteAllRemoteAudioStreams", _SE(js_cocos2dx_extension_agoraAudio_muteAllRemoteAudioStreams));
    cls->defineFunction("muteRemoteAudioStream", _SE(js_cocos2dx_extension_agoraAudio_muteRemoteAudioStream));
    cls->defineFunction("setDefaultAudioRouteToSpeakerphone", _SE(js_cocos2dx_extension_agoraAudio_setDefaultAudioRouteToSpeakerphone));
    cls->defineFunction("setEnableSpeakerphone", _SE(js_cocos2dx_extension_agoraAudio_setEnableSpeakerphone));

    cls->defineFinalizeFunction(_SE(js_agoraAudio_finalize));
    cls->install();

    js_cocos2dx_agoraAudio_prototype = cls->getProto();
    js_cocos2dx_agoraAudio_class = cls;
    
    se::ScriptEngine::getInstance()->clearException();
    return true;
}

bool register_jsb_agoraAudio(se::Object* obj)
{
    CCLOG("[Agora] register_jsb_agoraAudio");
    return js_register_cocos2dx_extension_agoraAudio(obj);
}

#endif 
