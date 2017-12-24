// main.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "VideoStitch.h"

int main()
{
	CamCapture camCapture;
	VideoStitch videoStitch;
	unsigned int camNum = 2;
	char dirName[80];

	//获取当期系统时间
	time_t tt = time(NULL);
	tm* t = localtime(&tt);

	//初始化文件夹名称
	sprintf(dirName, "Video_%d_%02d_%02d_%02d%02d%02d", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);

	//创建用于保存视频的目录
	if (_mkdir(dirName) == 0)
		printf("\nSuccessfully created the folder : .\\%s \n", dirName);
	else
		printf("\nCan not creat folder,the folder may already created.\n");

	camCapture.capture(camNum, 1280, 720, 24.0, dirName);

	videoStitch.readVideo(dirName);
	videoStitch.stitchVideo("outPut.avi");
    return 0;
}

