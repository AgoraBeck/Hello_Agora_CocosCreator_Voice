//
//  jsb_agoraAudio.cpp
//
//  Created by becklee on 18/3/3
//
//

#include "jsb_agoraAudio.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "scripting/js-bindings/manual/jsb_conversions.hpp"
#include "scripting/js-bindings/manual/jsb_global.h"
#include "cocos2d.h"

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

#if defined(__APPLE__)
     #include <AgoraAudioKit/IAgoraRtcEngine.h>
#elif defined(__ANDROID__)
    #include "IAgoraRtcEngine.h"
    // #include "../../../../../AgoraAuidoSDK/libs/Android/include/IAgoraRtcEngine.h"
    #include <android/log.h>
    #define LOG_TAG "beck-debug"
    #define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#else

#endif



#include <string.h>

using namespace cocos2d;
using namespace agora::rtc::cocos2dx;

class CAgoraAudioJsWrapper;
CAgoraAudioJsWrapper* g_SingleInstance;

se::Object *js_cocos2dx_agoraAudio_prototype = nullptr;
se::Class *js_cocos2dx_agoraAudio_class = nullptr;

class CAgoraAudioJsWrapper: public IRtcEngineEventHandler {
public:
//    static CAgoraAudioJsWrapper* getInstance();
    CAgoraAudioJsWrapper(){}

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
    virtual void onJoinChannelSuccess(const char* channel, uid_t userId, int elapsed) {
        (void)channel;
        (void)userId;
        (void)elapsed;
    }

    /**
    * when join channel success, the function will be called
    * @param [in] channel
    *        the channel name you have joined
    * @param [in] userId
    *        the userId of you in this channel
    * @param [in] elapsed
    *        the time elapsed in ms elapsed
    */
    virtual void onRejoinChannelSuccess(const char* channel, uid_t userId, int elapsed) {
        (void)channel;
        (void)userId;
        (void)elapsed;
    }

    /**
    * when warning message coming, the function will be called
    * @param [in] warn
    *        warning code
    * @param [in] msg
    *        the warning message
    */
    virtual void onWarning(int warn, const char* msg) {
        (void)warn;
        (void)msg;
    }

    /**
    * when error message come, the function will be called
    * @param [in] err
    *        error code
    * @param [in] msg
    *        the error message
    */
    virtual void onError(int err, const char* msg) {
        (void)err;
        (void)msg;
    }

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
    virtual void onAudioQuality(uid_t userId, int quality, unsigned short delay, unsigned short lost) {
        (void)userId;
        (void)quality;
        (void)delay;
        (void)lost;
    }

    /**
    * when the audio volume information come, the function will be called
    * @param [in] speakers
    *        the array of the speakers' audio volume information
    * @param [in] speakerNumber
    *        the count of speakers in this array
    * @param [in] totalVolume
    *        the total volume of all users
    */
    virtual void onAudioVolumeIndication(const AudioVolumeInfo* speakers, unsigned int speakerNumber, int totalVolume) {
        (void)speakers;
        (void)speakerNumber;
        (void)totalVolume;
    }

    /**
    * when the audio volume information come, the function will be called
    * @param [in] stats
    *        the statistics of the call
    */
    virtual void onLeaveChannel(const RtcStats& stats) {
        (void)stats;
    }

    /**
    * when the information of the RTC engine stats come, the function will be called
    * @param [in] stats
    *        the RTC engine stats
    */
    virtual void onRtcStats(const RtcStats& stats) {
        (void)stats;
    }

    /**
    * when the audio device state changed(plugged or removed), the function will be called
    * @param [in] deviceId
    *        the ID of the state changed audio device
    * @param [in] deviceType
    *        the type of the audio device(playout device or record device)
    * @param [in] deviceState
    *        the device is been removed or added
    */
    virtual void onAudioDeviceStateChanged(const char* deviceId, int deviceType, int deviceState) {
        (void)deviceId;
        (void)deviceType;
        (void)deviceState;
    }

    /**
     * When audio mixing file playback finished, this function will be called
     */
    virtual void onAudioMixingFinished() {
    }

    /**
     * When far-end rhythm begins/ends, these functions will be called
     */
    virtual void onRemoteAudioMixingBegin() {
    }
    virtual void onRemoteAudioMixingEnd() {
    }

    /**
    * When audio effect playback finished, this function will be called
    */
    virtual void onAudioEffectFinished(int soundId) {
    }

    /**
    * when the video device state changed(plugged or removed), the function will be called
    * @param [in] deviceId
    *        the ID of the state changed video device
    * @param [in] deviceType
    *        not used
    * @param [in] deviceState
    *        the device is been removed or added
    */
    virtual void onVideoDeviceStateChanged(const char* deviceId, int deviceType, int deviceState) {
        (void)deviceId;
        (void)deviceType;
        (void)deviceState;
    }

    /**
    * report the network quality
	* @param [in] userId
	*        the userId of the remote user
	* @param [in] txQuality
    *        the score of the send network quality 0~5 the higher the better
	* @param [in] rxQuality
	*        the score of the recv network quality 0~5 the higher the better
	*/
    virtual void onNetworkQuality(uid_t userId, int txQuality, int rxQuality) {
        (void)userId;
		(void)txQuality;
		(void)rxQuality;
    }

    /**
    * report the last-mile test network quality
    * @param [in] quality
    *        the score of the network quality 0~5 the higher the better
    */
    virtual void onLastmileQuality(int quality) {
        (void)quality;
    }

    /**
    * when the first local video frame displayed, the function will be called
    * @param [in] width
    *        the width of the video frame
    * @param [in] height
    *        the height of the video frame
    * @param [in] elapsed
    *        the time elapsed from channel joined in ms
    */
    // virtual void onFirstLocalVideoFrame(int width, int height, int elapsed) {
    //     (void)width;
    //     (void)height;
    //     (void)elapsed;
    // }

    /**
    * when the first remote video frame decoded, the function will be called
    * @param [in] userId
    *        the userId of the remote user
    * @param [in] width
    *        the width of the video frame
    * @param [in] height
    *        the height of the video frame
    * @param [in] elapsed
    *        the time elapsed from channel joined in ms
    */
    // virtual void onFirstRemoteVideoDecoded(uid_t userId, int width, int height, int elapsed) {
    //     (void)userId;
    //     (void)width;
    //     (void)height;
    //     (void)elapsed;
    // }

    /**
     * when video size changed or rotation changed, the function will be called
     * @param [in] userId
     *        the userId of the remote user or local user (0)
     * @param [in] width
     *        the new width of the video
     * @param [in] height
     *        the new height of the video
     * @param [in] rotation
     *        the rotation of the video
     */
    // virtual void onVideoSizeChanged(uid_t userId, int width, int height, int rotation) {
    //     (void)userId;
    //     (void)width;
    //     (void)height;
    //     (void)rotation;
    // }
    
    // virtual void onRemoteVideoStateChanged(uid_t userId, REMOTE_VIDEO_STATE state) {
    //     (void)userId;
    //     (void)state;
    // }

    /**
    * when the first remote video frame displayed, the function will be called
    * @param [in] userId
    *        the userId of the remote user
    * @param [in] width
    *        the width of the video frame
    * @param [in] height
    *        the height of the video frame
    * @param [in] elapsed
    *        the time elapsed from remote user called joinChannel in ms
    */
    // virtual void onFirstRemoteVideoFrame(uid_t userId, int width, int height, int elapsed) {
    //     (void)userId;
    //     (void)width;
    //     (void)height;
    //     (void)elapsed;
    // }

    /**
    * when any other user joined in the same channel, the function will be called
    * @param [in] userId
    *        the userId of the remote user
    * @param [in] elapsed
    *        the time elapsed from remote used called joinChannel to joining completed in ms
    */
    virtual void onUserJoined(uid_t userId, int elapsed) {
        (void)userId;
        (void)elapsed;
    }

    /**
    * when user offline(exit channel or offline by accident), the function will be called
    * @param [in] userId
    *        the userId of the remote user
    */
    virtual void onUserOffline(uid_t userId, USER_OFFLINE_REASON_TYPE reason) {
        (void)userId;
        (void)reason;
    }

    /**
    * when remote user muted the audio stream, the function will be called
    * @param [in] userId
    *        the userId of the remote user
    * @param [in] muted
    *        true: the remote user muted the audio stream, false: the remote user unmuted the audio stream
    */
    virtual void onUserMuteAudio(uid_t userId, bool muted) {
        (void)userId;
        (void)muted;
    }

    /**
    * when remote user muted the video stream, the function will be called
    * @param [in] userId
    *        the userId of the remote user
    * @param [in] muted
    *        true: the remote user muted the video stream, false: the remote user unmuted the video stream
    */
    // virtual void onUserMuteVideo(uid_t userId, bool muted) {
    //     (void)userId;
    //     (void)muted;
    // }

	/**
	* when remote user enable video function, the function will be called
	* @param [in] userId
	*        the userId of the remote user
	* @param [in] enabled
	*        true: the remote user has enabled video function, false: the remote user has disabled video function
	*/
    // virtual void onUserEnableVideo(uid_t userId, bool enabled) {
    //     (void)userId;
	// 	(void)enabled;
	// }
	
	/**
    * when remote user enable local video function, the function will be called
    * @param [in] userId
    *        the userId of the remote user
    * @param [in] enabled
    *        true: the remote user has enabled local video function, false: the remote user has disabled local video function
    */
    // virtual void onUserEnableLocalVideo(uid_t userId, bool enabled) {
    //     (void)userId;
    //     (void)enabled;
    // }
    
    /**
    * when api call executed completely, the function will be called
    * @param [in] api
    *        the api name
    * @param [in] err
    *        error code while 0 means OK
    */
    virtual void onApiCallExecuted(int err, const char* api, const char* result) {
        (void)err;
        (void)api;
        (void)result;
    }

    /**
	* reported local video stats
	* @param [in] stats
    *        the latest local video stats
    */
	// virtual void onLocalVideoStats(const LocalVideoStats& stats) {
	// 	(void)stats;
    // }

    /**
    * reported remote video stats
    * @param [in] stats
	*        the latest remote video stats
	*/
	// virtual void onRemoteVideoStats(const RemoteVideoStats& stats) {
	// 	(void)stats;
    // }

    /**
    * when the camera is ready to work, the function will be called
    */
    // virtual void onCameraReady() {}

    // virtual void onCameraFocusAreaChanged(int x, int y, int width, int height) {
        (void)x;
        (void)y;
        (void)width;
        (void)height;
    }

    /**
    * when all video stopped, the function will be called then you can repaint the video windows
    */
    // virtual void onVideoStopped() {}

    /**
    * when the network can not worked well, the function will be called
    */
    virtual void onConnectionLost() {}

    /**
    * when local user disconnected by accident, the function will be called(then SDK will try to reconnect itself)
    */
    virtual void onConnectionInterrupted() {}
    
    /**
     * when local user is banned by the server, the function will be called
     */
    virtual void onConnectionBanned() {}
    
    virtual void onRefreshRecordingServiceStatus(int status) {
        (void)status;
    }

//    virtual void onStreamError(int streamId, int code, int parameter, const char* message, size_t length) {}
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
    virtual void onStreamMessage(uid_t userId, int streamId, const char* data, size_t length) {
        (void)userId;
        (void)streamId;
        (void)data;
        (void)length;
    }

	/**
	* 
	*/
    virtual void onStreamMessageError(uid_t userId, int streamId, int code, int missed, int cached) {
        (void)userId;
        (void)streamId;
        (void)code;
        (void)missed;
        (void)cached;
    }

    virtual void onMediaEngineLoadSuccess() {
    }
    virtual void onMediaEngineStartCallSuccess() {
    }
    /**
    * when token is enabled, and specified token is invalid or expired, this function will be called.
    * APP should generate a new token and call renewToken() to refresh the token.
    * NOTE: to be compatible with previous version, ERR_TOKEN_EXPIRED and ERR_INVALID_TOKEN are also reported via onError() callback.
    * You should move renew of token logic into this callback.
    */
    virtual void onRequestToken() {
    }

    /**
    * when the first local audio frame generated, the function will be called
    * @param [in] elapsed
    *        the time elapsed from remote user called joinChannel in ms
    */
    virtual void onFirstLocalAudioFrame(int elapsed) {
        (void)elapsed;
    }

    /**
    * when the first remote audio frame arrived, the function will be called
    * @param [in] userId
    *        the userId of the remote user
    * @param [in] elapsed
    *        the time elapsed from remote user called joinChannel in ms
    */
    virtual void onFirstRemoteAudioFrame(uid_t userId, int elapsed) {
        (void)userId;
        (void)elapsed;
    }
    /** @param [in] userId
    *        the speaker userId who is talking in the channel
    */
    virtual void onActiveSpeaker(uid_t userId) {
        (void)userId;
    }

    /**
    * when client role is successfully changed, the function will be called
    */
    virtual void onClientRoleChanged(CLIENT_ROLE_TYPE oldRole, CLIENT_ROLE_TYPE newRole) {
    }

    virtual void onAudioDeviceVolumeChanged(MEDIA_DEVICE_TYPE deviceType, int volume, bool muted) {
        (void)deviceType;
        (void)volume;
        (void)muted;
    }

    virtual void onStreamPublished(const char *url, int error) {
        (void)url;
        (void)error;
    }

    virtual void onStreamUnpublished(const char *url) {
        (void)url;
    }

    virtual void onTranscodingUpdated() {
    }

    virtual void onStreamInjectedStatus(const char* url, uid_t userId, int status) {
        (void)url;
        (void)userId;
        (void)status;
    }

	virtual void onAudioRoutingChanged(int routing) {
		(void)routing;
	}

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
        unsigned short rxKBitRate) {
        (void)uid;
        (void)delay;
        (void)lost;
        (void)rxKBitRate;
    }

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
	virtual void onMicrophoneEnabled(bool enabled) {
		(void)enabled;
	}
};

void CAgoraAudioJsWrapper::onJoinChannelSuccess(const char* channel, uid_t uid, int elapsed)
{
    CCLOG("[Agora]:onJoinChannelSuccess %s, %u, %d", channel, uid, elapsed);
    static char channelName[0x100];
    strcpy(channelName, channel);

    cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
        
        se::Value func;
        if (_refObj->getProperty("onJoinChannelSuccess", &func)) {
            se::ScriptEngine::getInstance()->clearException();
            se::AutoHandleScope hs;
            
            se::ValueArray args;
            args.push_back(se::Value(channelName));
            args.push_back(se::Value((uid_t)uid));
            args.push_back(se::Value((int)elapsed));
          
            func.toObject()->call(args, _refObj);
        }
    });
    
    return ;
}

void CAgoraAudioJsWrapper:: onLeaveChannel(const RtcStats& stats)
{
    CCLOG("[Agora]:onLeaveChannel %d, %d, %d", stats.totalDuration, stats.txBytes, stats.rxBytes);
    clear();
    
    cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
        se::Value func;
        if(_refObj->getProperty("onLeaveChannel", &func)){
            se::ScriptEngine::getInstance()->clearException();
            se::AutoHandleScope hs;
            
            se::ValueArray args;
            args.push_back(se::Value((int)stats.totalDuration));
            args.push_back(se::Value((int)stats.txBytes));
            args.push_back(se::Value((int)stats.rxBytes));
            args.push_back(se::Value((int)stats.txKBitRate));
            args.push_back(se::Value((int)stats.rxKBitRate));
            args.push_back(se::Value((int)stats.txAudioKBitRate));
            args.push_back(se::Value((int)stats.rxAudioKBitRate));
            args.push_back(se::Value((int)stats.txVideoKBitRate));
            args.push_back(se::Value((int)stats.rxVideoKBitRate));
            args.push_back(se::Value((int)stats.users));
            args.push_back(se::Value((double)stats.cpuTotalUsage));
            args.push_back(se::Value((double)stats.cpuAppUsage));
            
            func.toObject()->call(args, _refObj);
        }
    });
    
    return ;
}


void CAgoraAudioJsWrapper::onJoinChannelSuccess(const char* channel, uid_t userId, int elapsed) {
    (void)channel;
    (void)userId;
    (void)elapsed;
}


void CAgoraAudioJsWrapper::onRejoinChannelSuccess(const char* channel, uid_t userId, int elapsed) {
    (void)channel;
    (void)userId;
    (void)elapsed;
}


void CAgoraAudioJsWrapper::onWarning(int warn, const char* msg) {
    (void)warn;
    (void)msg;
}

void CAgoraAudioJsWrapper::onError(int err, const char* msg) {
    (void)err;
    (void)msg;
}

void CAgoraAudioJsWrapper::onAudioQuality(uid_t userId, int quality, unsigned short delay, unsigned short lost) {
    (void)userId;
    (void)quality;
    (void)delay;
    (void)lost;
}


void CAgoraAudioJsWrapper::onAudioVolumeIndication(const AudioVolumeInfo* speakers, unsigned int speakerNumber, int totalVolume) {
    (void)speakers;
    (void)speakerNumber;
    (void)totalVolume;
}

void CAgoraAudioJsWrapper::onLeaveChannel(const RtcStats& stats) {
    (void)stats;
}


void CAgoraAudioJsWrapper::onRtcStats(const RtcStats& stats) {
    (void)stats;
}


void CAgoraAudioJsWrapper::onAudioDeviceStateChanged(const char* deviceId, int deviceType, int deviceState) {
    (void)deviceId;
    (void)deviceType;
    (void)deviceState;
}


void CAgoraAudioJsWrapper::onAudioMixingFinished() {
}

void CAgoraAudioJsWrapper::onRemoteAudioMixingBegin() {
}
void CAgoraAudioJsWrapper::onRemoteAudioMixingEnd() {
}


void CAgoraAudioJsWrapper::onAudioEffectFinished(int soundId) {
}


void CAgoraAudioJsWrapper::onVideoDeviceStateChanged(const char* deviceId, int deviceType, int deviceState) {
    (void)deviceId;
    (void)deviceType;
    (void)deviceState;
}


void CAgoraAudioJsWrapper::onNetworkQuality(uid_t userId, int txQuality, int rxQuality) {
    (void)userId;
	(void)txQuality;
	(void)rxQuality;
}

void CAgoraAudioJsWrapper::onLastmileQuality(int quality) {
    (void)quality;
}

void CAgoraAudioJsWrapper::onUserJoined(uid_t userId, int elapsed) {
    (void)userId;
    (void)elapsed;
}


void CAgoraAudioJsWrapper::onUserOffline(uid_t userId, USER_OFFLINE_REASON_TYPE reason) {
    (void)userId;
    (void)reason;
}


void CAgoraAudioJsWrapper::onUserMuteAudio(uid_t userId, bool muted) {
    (void)userId;
    (void)muted;
}


void CAgoraAudioJsWrapper::onApiCallExecuted(int err, const char* api, const char* result) {
    (void)err;
    (void)api;
    (void)result;
}

void CAgoraAudioJsWrapper::onConnectionLost() {}


void CAgoraAudioJsWrapper::onConnectionInterrupted() {}


void CAgoraAudioJsWrapper::onConnectionBanned() {}

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

void CAgoraAudioJsWrapper::onMediaEngineLoadSuccess() {
}
void CAgoraAudioJsWrapper::onMediaEngineStartCallSuccess() {
}
/**
* when token is enabled, and specified token is invalid or expired, this function will be called.
* APP should generate a new token and call renewToken() to refresh the token.
* NOTE: to be compatible with previous version, ERR_TOKEN_EXPIRED and ERR_INVALID_TOKEN are also reported via onError() callback.
* You should move renew of token logic into this callback.
*/
void CAgoraAudioJsWrapper::onRequestToken() {
}


void CAgoraAudioJsWrapper::onFirstLocalAudioFrame(int elapsed) {
    (void)elapsed;
}


void CAgoraAudioJsWrapper::onFirstRemoteAudioFrame(uid_t userId, int elapsed) {
    (void)userId;
    (void)elapsed;
}

void CAgoraAudioJsWrapper::onActiveSpeaker(uid_t userId) {
    (void)userId;
}


void CAgoraAudioJsWrapper::onClientRoleChanged(CLIENT_ROLE_TYPE oldRole, CLIENT_ROLE_TYPE newRole) {
}

void CAgoraAudioJsWrapper::onAudioDeviceVolumeChanged(MEDIA_DEVICE_TYPE deviceType, int volume, bool muted) {
    (void)deviceType;
    (void)volume;
    (void)muted;
}

void CAgoraAudioJsWrapper::onStreamPublished(const char *url, int error) {
    (void)url;
    (void)error;
}

void CAgoraAudioJsWrapper::onStreamUnpublished(const char *url) {
    (void)url;
}

void CAgoraAudioJsWrapper::onTranscodingUpdated() {
}

void CAgoraAudioJsWrapper::onStreamInjectedStatus(const char* url, uid_t userId, int status) {
    (void)url;
    (void)userId;
    (void)status;
}

void CAgoraAudioJsWrapper::onAudioRoutingChanged(int routing) {
	(void)routing;
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
    CCLOG("[Agora] js_cocos2dx_extension_agoraAudio_initialize \n");
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
        ctx.appId = appId;
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
    CCLOG("[Agora] js_cocos2dx_extension_agoraAudio_joinChannel \n");
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
   
        int ret = cobj->joinChannel(token, channelId, info, uid);
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

static bool js_cocos2dx_extension_agoraAudio_setParameters(se::State& s) {
    CCLOG("setParameters() !!!");
    IRtcEngine* cobj = (IRtcEngine*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_extension_agoraAudio_setParameters: Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 1)
    {
        std::string strVal;
        ok &= seval_to_std_string(args[0], &strVal);
        
        int ret = cobj->setParameters(strVal.c_str());
        
        int32_to_seval(ret, &s.rval());
        
        SE_PRECONDITION2(ok, false, "js_cocos2dx_extension_agoraAudio_setParameters : Error processing arguments");
        return true;
    }
    
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 1);
    return false;
    
}
SE_BIND_FUNC(js_cocos2dx_extension_agoraAudio_setParameters)

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
    
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 0);
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
    
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 0);
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
    
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 0);
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
    CCLOG("[Agora] constructor \n");
    const auto& args = s.args();
    
    size_t argc = args.size();
    
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
    CCLOG("[Agora] js_register_cocos2dx_extension_agoraAudio \n");
    
    auto cls = se::Class::create("agoraAudio", obj, nullptr, _SE(js_cocos2dx_extension_agoraAudio_constructor));
    
    cls->defineFunction("initialize", _SE(js_cocos2dx_extension_agoraAudio_initialize));
    cls->defineFunction("joinChannel", _SE(js_cocos2dx_extension_agoraAudio_joinChannel));
    cls->defineFunction("leaveChannel", _SE(js_cocos2dx_extension_agoraAudio_leaveChannel));
    cls->defineFunction("getVersion", _SE(js_cocos2dx_extension_agoraAudio_getVersion));
    cls->defineFunction("getErrorDescription", _SE(js_cocos2dx_extension_agoraAudio_getErrorDescription));
    cls->defineFunction("setLogFilter", _SE(js_cocos2dx_extension_agoraAudio_setLogFilter));
    cls->defineFunction("setLogFile", _SE(js_cocos2dx_extension_agoraAudio_setLogFile));
    cls->defineFunction("setParameters", _SE(js_cocos2dx_extension_agoraAudio_setParameters));
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
    CCLOG("[Agora] register_jsb_agoraAudio \n");
    return js_register_cocos2dx_extension_agoraAudio(obj);
}

#endif 
