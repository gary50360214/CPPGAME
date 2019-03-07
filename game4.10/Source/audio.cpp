/*
 * gamelib.cpp: 本檔案儲存支援遊戲相關的class的implementation
*/

#include "stdafx.h"
#include "game.h"
#include "MainFrm.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include <direct.h>
#include <string.h>
#include "gamelib.h"
#include "audio.h"

namespace game_framework
{

/////////////////////////////////////////////////////////////////////////////
// CAudio
// 這個class提供音效(Wave及MIDI)
// 要懂得怎麼呼叫(運用)其各種能力，但是可以不懂下列的程式是什麼意思
// Note: The following memebers of CAudio are designed to be graceful so
//    that the game does not break down, when NO audio device is ready.
/////////////////////////////////////////////////////////////////////////////

CAudio CAudio::audio;

CAudio::CAudio()
{
    const int PIPE_SIZE = 400;
    isOpened = false;
    // Create a pipe and a thread for MCI commands
    DWORD dwThreadID;
    HANDLE hReadEnd;
    BOOL bRet = CreatePipe(&hReadEnd, &hWriteEnd, NULL, PIPE_SIZE);
    GAME_ASSERT(bRet == TRUE, "CAudio error: CreatePipe failed!");
    hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) MCIThread, (LPVOID)hReadEnd, 0, &dwThreadID);
    GAME_ASSERT(hThread != NULL, "CAudio error: Create Thread failed!");
}

CAudio::~CAudio()
{
    if (isOpened)
        Close();

    // Tell MCI thread to exit thread
    CloseHandle(hWriteEnd);
    // Wait for MCI thread to exit
    WaitForSingleObject(hThread, INFINITE);
    TRACE("~CAudio()\n");
}

void CAudio::ExecuteMciCommand(char* command)
{
    bool shouldExecuteCommand = true;
    char action[20], device[20];
    int ret = sscanf(command, "%s %s", action, device);
    GAME_ASSERT(ret == 2, "MCI command error: internal error!");

    if (strcmp(action, "pause") == 0)
    {
        // pause command should be executed only if the device is playing
        char statusCommand[MAX_MCI_COMMAND_SIZE], rval[200];
        sprintf(statusCommand, "status %s mode", device);
        mciSendString(statusCommand, rval, 200, NULL);
        shouldExecuteCommand = strcmp(rval, "playing") == 0;
    }
    else if (strcmp(action, "resume") == 0)
    {
        // resume command should be executed only if the device is paused
        char statusCommand[MAX_MCI_COMMAND_SIZE], rval[200];
        sprintf(statusCommand, "status %s mode", device);
        mciSendString(statusCommand, rval, 200, NULL);
        shouldExecuteCommand = strcmp(rval, "paused") == 0;
    }

    //
    // Ready to execute the assigned command
    //
    if (shouldExecuteCommand)
    {
        bool success = mciSendString(command, NULL, 0, NULL) == 0;

        //
        // Report MCI command problems, if the command is not successful.
        //
        if (!success)
        {
            string error("MCI command error: the following command does not work\n\n\"");
            GAME_ASSERT(false, error.append(command).append("\"").c_str());
        }
    }
}

void CAudio::MCIThread(HANDLE hRead)
{
    const int MAX_BUFFER_SIZE = 400;
    char buf[MAX_BUFFER_SIZE];
    DWORD dwRead;
    char* ptr = buf;

    //
    // ReadFile returns false when the pipe is closed
    //
    while (ReadFile(hRead, ptr, buf + MAX_BUFFER_SIZE - ptr - 1, &dwRead, NULL))
    {
        GAME_ASSERT((buf + MAX_BUFFER_SIZE - ptr - 1) > 0, "MCI Command Internal error: buffer size is too small!");
        char* ptr_end = ptr + dwRead;
        *ptr_end = '\0';	// add a termination so that the buffer can be printed out as a string
        ptr = buf;			// always process from the beginning of buf

        // TRACE("ReadFile: %s\n", ptr);
        //
        // It is possible that several commands are sent together.
        // A command is encapsulated in a pair of square brackets, like [command].
        //
        while (ptr < ptr_end)
        {
            GAME_ASSERT(ptr[0] == '[', "Internal error: incorrect MCI command format");
            // search for "]"
            int i;

            for (i = 0; (ptr + i) < (ptr_end) && ptr[i] != ']'; i++)
                ;

            //
            //  Check if the command is complete ("]" has been received).
            //
            if (ptr[i] == ']')
            {
                // Found a complete command
                ptr[i] = '\0';
                // TRACE("Received: %s\n", ptr+1);
                ExecuteMciCommand(ptr + 1);
                ptr += (i + 1);	// advance to the next command
            }
            else
            {
                // The command does not end with ']'. Copy the incomplete commmand to
                //   the head of buf[] and set ptr to the point to be read again.
                memcpy(buf, ptr, ptr_end - ptr);
                ptr = buf + (ptr_end - ptr);
                ptr_end = ptr + 1; // force ptr != ptr_end
                break;
            }
        }

        // After the end of a correct command, reset ptr to buf
        if (ptr == ptr_end)
            ptr = buf;
    }

    // pipe closed
    CloseHandle(hRead);
    TRACE("Exit MCI command thread\n");
}

void CAudio::SendMciCommand(char* command)
{
    char buf[MAX_MCI_COMMAND_SIZE + 2];
    DWORD dwWrite;
    sprintf(buf, "[%s]", command);
    BOOL bRet = WriteFile(hWriteEnd, buf, strlen(buf), &dwWrite, NULL);
    GAME_ASSERT(bRet != NULL, "CAudio Error: cannot write to command pipe!");
}

void CAudio::Close()
{
    if (!isOpened)
        return;

    map<int, Info>::iterator i;

    for (i = info.begin(); i != info.end(); i++)
    {
        if (i->second.isGood)
        {
            char command[MAX_MCI_COMMAND_SIZE];
            sprintf(command, "close device%d", i->first);
            SendMciCommand(command);
        }
    }
}

CAudio* CAudio::Instance()
{
    return &audio;
}

bool CAudio::Load(unsigned id, char* lpzFileName)
{
    if (!isOpened)
        return false;

    //
    // Make sure the audio file has not been loaded yet.
    //
    map<int, Info>::iterator i = info.find(id);

    if (i != info.end())
    {
        string str;
        str.append("Can not load audio file \"").append(lpzFileName);
        GAME_ASSERT(false, str.append("\": the same audio ID cannot be loaded twice!").c_str());
    }

    //
    // Get the file type and convert it to upper case letters
    //
    string file_type(&lpzFileName[strlen(lpzFileName) - 4]);

    for (unsigned i = 0; i < file_type.size(); i++)
        file_type[i] = toupper(file_type[i]);

    if (file_type.compare(".MP3") != 0 &&
            file_type.compare(".WAV") != 0 &&
            file_type.compare(".MID") != 0)
    {
        string str;
        str.append("Can not load audio file ").append(lpzFileName);
        GAME_ASSERT(false, str.append(": file type is not supported!").c_str());
    }

    info.insert(map<int, Info>::value_type(id, Info()));
    info[id].fileName.assign(lpzFileName);
    char command[MAX_MCI_COMMAND_SIZE];
    sprintf(command, "open %s type mpegvideo alias device%d", lpzFileName, id);
    //
    // Make sure the file exists.
    //
    FILE* fp = fopen(lpzFileName, "rb");

    if (fp == NULL)
    {
        string str;
        str.append("Can not load audio file \"").append(lpzFileName).append("\": the file does not exist!");
        GAME_ASSERT(false, str.c_str());
    }

    fclose(fp);
    //
    // Load the audio file
    //
    SendMciCommand(command);
    info[id].isGood = true;
    return true;
}

bool CAudio::Open()
{
    GAME_ASSERT(!isOpened, "Audio device can not be opened twice!");

    if (!ENABLE_AUDIO)
        return false;

    //
    // Test if MCI command can be sent. The testing does not need to be done with the MCIThread.
    //
    isOpened = mciSendString("open mpegvideo", NULL, 0, NULL) == 0;

    if (isOpened)
        mciSendString("close mpegvideo", NULL, 0, NULL);

    return isOpened;
}

void CAudio::Pause()
{
    if (!isOpened)
        return;

    map<int, Info>::iterator i;

    for (i = info.begin(); i != info.end(); i++)
    {
        if (i->second.isGood)
        {
            char command[MAX_MCI_COMMAND_SIZE];
            sprintf(command, "pause device%d wait", i->first);
            SendMciCommand(command);
        }
    }
}

void CAudio::Play(unsigned id, bool repeat_flag)
{
    if (!isOpened)
        return;

    GAME_ASSERT(info.find(id) != info.end(), "Can not play back audio: incorrect Audio ID!");

    //
    // Do not play, if the file is no good
    //
    if (!info[id].isGood)
        return;

    char command[MAX_MCI_COMMAND_SIZE];

    if (repeat_flag)
        sprintf(command, "play device%d from 0 repeat", id);
    else
        sprintf(command, "play device%d from 0", id);

    SendMciCommand(command);
}

void CAudio::SetPowerResume()
{
    // Force Pause operation when the system power is to be turned off
    Pause();
}

void CAudio::Resume()
{
    if (!isOpened)
        return;

    map<int, Info>::iterator i;

    for (i = info.begin(); i != info.end(); i++)
    {
        char command[MAX_MCI_COMMAND_SIZE];
        sprintf(command, "resume device%d", i->first);
        SendMciCommand(command);
    }
}

void CAudio::Stop(unsigned id)
{
    if (!isOpened)
        return;

    GAME_ASSERT(info.find(id) != info.end(), "Can not stop audio: incorrect Audio ID!");

    if (info[id].isGood)
    {
        char command[MAX_MCI_COMMAND_SIZE];
        sprintf(command, "stop device%d", id);
        SendMciCommand(command);
    }
}

}