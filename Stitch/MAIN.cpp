//main.cpp
#include "stdafx.h"
#include "timedetection.h"
#include "StitchFrame.h"


int main()
{
	cout << "CV_VERSION: " << CV_VERSION << endl;


	StitchFrame stitch_1CR, stitch_1LC, stitch_2CR, stitch_2LC, stitch_UD;	//实例化的StitchFrame类的对象
	int flag = 1;


	//载入原图
	stitch_1CR.setSRC_L("src\\1C.jpg");
	stitch_1CR.setSRC_R("src\\1R.jpg").findH("H\\H_1CR.xml", STITCH_SURF, IS_REBUILD_H);
	
	stitch_1LC.setSRC_L("src\\1L.jpg");
	stitch_1LC.setSRC_R("src\\1C.jpg").findH("H\\H_1LC.xml", STITCH_SURF, IS_REBUILD_H);
	
	stitch_2CR.setSRC_L("src\\2C.jpg");
	stitch_2CR.setSRC_R("src\\2R.jpg").findH("H\\H_2CR.xml", STITCH_SURF, IS_REBUILD_H);
	
	stitch_2LC.setSRC_L("src\\2L.jpg");
	stitch_2LC.setSRC_R("src\\2C.jpg").findH("H\\H_2LC.xml", STITCH_SURF, IS_REBUILD_H);


	//拼接
	//clock_t start, end;

	TimeDetection time;

	while (1)
	{
		//start = getTickCount();
		time.setStartPos();
		stitch_1CR.stitch(50);
		stitch_1LC.setSRC_R(stitch_1CR.getResult());				//如果在这里再findH会如何？
		imshow("stitch_1_whole", stitch_1LC.stitch(50));
		stitch_2CR.stitch(50);
		stitch_2LC.setSRC_R(stitch_2CR.getResult());
		imshow("stitch_2_whole", stitch_2LC.stitch(50));

		stitch_UD.setSRC_L(stitch_1LC.getResult());
		stitch_UD.setSRC_R(stitch_2LC.getResult()).findH("H\\H_UD.xml", STITCH_SURF, false);
		
		imwrite("stitch_whole.jpg", stitch_UD.stitch_v(100));
		imshow("stitch_whole", stitch_UD.getResult());
		time.setEndPos().getAvgTime().getAvgFps().getCurTime();
		
		//end = getTickCount();
		//cout << (double)((end - start)/getTickFrequency()*1000)<< "ms" << endl << endl;
		waitKey(50);
	}

	cvWaitKey(0);



	//若存在单应性矩阵则导入，否则计算，默认情况 (isRebuild=true) 下每次运行都会重新计算 H

	//stitch_1CR.findH("H\\H_1CR.xml", STITCH_SURF, false);
	//stitch_1LC.findH("H\\H_1LC.xml", STITCH_SURF, false);
	//stitch_2CR.findH("H\\H_2CR.xml", STITCH_SURF, false);
	//stitch_2LC.findH("H\\H_2LC.xml", STITCH_SURF, false);


	//计算单应性矩阵
	
	/****************************************************************************************
	if(!stitch_1CR.setH("H\\H_1CR.xml"))
	{
		cout << "H=  (PYTHON)" << endl << format(stitch_1CR.getH(), Formatter::FMT_PYTHON) << endl << endl;
		stitch_1CR.findH("H\\H_1CR.xml",STITCH_SURF,false);
		stitch_1CR.show("1CR_H");
		cout << "H=  (PYTHON)" << endl << format(stitch_1CR.getH(), Formatter::FMT_PYTHON) << endl << endl;
		
	}
	
	if (!stitch_1LC.setH("H\\H_1LC.xml"))
	{
		stitch_1LC.findH("H\\H_1LC.xml",STITCH_SURF,false);
		stitch_1LC.show("1LC_H");
	}
	
	if (!stitch_2CR.setH("H\\H_2CR.xml"))
	{
		stitch_2CR.findH("H\\H_2CR.xml", STITCH_SURF,false);
		stitch_2CR.show("2CR_H");
	}
	
	if (!stitch_2LC.setH("H\\H_2LC.xml"))
	{
		stitch_2LC.findH("H\\H_2LC.xml", STITCH_SURF,false);
		stitch_2LC.show("2LC_H");
		
	}
	
	***********************************************************************************************/




	//摄像头拼接

	//VideoCapture capture(0);
	//Mat frame,src;
	//StitchFrame video;
	//Time time;
	//capture >> frame;
	//imwrite("src/src_frame.jpg", frame);
	//getchar();
	//src = imread("src/src_frame.jpg");
	//capture >> frame;
	//video.setSRC_L(src);
	//video.setSRC_R(frame);
	//video.findH("H/H_frame.xml");
	//video.show("H");

	//while(1)
	//{
	//	time.setStartPos();
	//	capture >> frame;
	//	video.setSRC_R(frame);
	//	imshow("capture", video.stitch(30));
	//	time.setEndPos();
	//	time.getAvgTime();
	//	time.getCurTime();
	//	waitKey(40);
	//}
	//
	//





/********************************************************************************************
	Mat gray_R, gray_C,gray_L;
	start = clock();
	cvtColor(src_R, gray_R, COLOR_BGR2GRAY);
	cvtColor(src_C, gray_C, COLOR_BGR2GRAY);
	cvtColor(src_L, gray_L, COLOR_BGR2GRAY);
	Ptr<SURF> surf;
	surf = SURF::create(800);
	BFMatcher matcher;
	Mat c, d, e;
	vector<KeyPoint>key1, key2,key3;
	vector<DMatch> matches_CR ,matches_LC;
	surf->detectAndCompute(gray_R, Mat(), key1, c);
	surf->detectAndCompute(gray_C, Mat(), key2, d);
	surf->detectAndCompute(gray_L, Mat(), key3, e);
	matcher.match(c, d, matches_CR);       
	matcher.match(d, e, matches_LC);
	sort(matches_CR.begin(), matches_CR.end()); 
	sort(matches_LC .begin(), matches_LC.end());
	vector< DMatch > good_matches_CR, good_matches_LC;
	int ptsPairs = min(50, (int)(matches_CR.size() * 0.15));
	cout << ptsPairs << endl;
	for (int i = 0; i < ptsPairs; i++)
	{
		good_matches_CR.push_back(matches_CR[i]);
	}
	ptsPairs = min(50, (int)(matches_LC.size() * 0.15));
	cout << ptsPairs << endl;
	for (int i = 0; i < ptsPairs; i++)
	{
		good_matches_LC.push_back(matches_LC[i]);
	}
	Mat outimg_CR;
	drawMatches(gray_R, key1, gray_C, key2, good_matches_CR, outimg_CR, Scalar::all(-1), Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS); 
	Mat outimg_LC;
	drawMatches(gray_C, key2, gray_L, key3, good_matches_LC, outimg_LC, Scalar::all(-1), Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);  //绘制匹配点
	vector<Point2f> obj;
	vector<Point2f> scene;
	for (size_t i = 0; i < good_matches_CR.size(); i++)
	{
		obj.push_back(key1[good_matches_CR[i].queryIdx].pt);
		scene.push_back(key2[good_matches_CR[i].trainIdx].pt);
	}
	Mat H_CR = findHomography(obj, scene, RANSAC);      
	obj.clear();
	scene.clear();
	for (size_t i = 0; i < good_matches_LC.size(); i++)
	{
		obj.push_back(key2[good_matches_LC[i].queryIdx].pt);
		scene.push_back(key3[good_matches_LC[i].trainIdx].pt);
	}
	Mat H_LC = findHomography(obj, scene, RANSAC);
	finish = clock();
	duration = (double)(finish - start);
	printf("%.2f ms\n", duration);
	
	*****************************************************************************/

	
	
	

	//拼接

	/*****************************************************************
	clock_t begin, end;
	int width = 30;//渐入渐出融合宽度
	Mat result,result_LC;
	//while (1) {
	begin = clock();
		//cols　列 rows 行
		warpPerspective(src_R, result,H_CR, Size(3 * src_R.cols - width, src_R.rows));//Size设置结果图像宽度，宽度裁去一部分，e可调
		
		Mat half(result, Rect(0, 0, src_C.cols - width, src_C.rows));
		//imshow("half", half);
		src_C(Range::all(), Range(0, src_C.cols - width)).copyTo(half);
		//imshow("half2", half);
		for (int i = 0; i < width; i++)
		{
			result.col(src_C.cols - width + i) = (width - i) / (float)width*src_C.col(src_C.cols - width + i) + i / (float)width*result.col(src_C.cols - width + i);  //权重
		}
	end = clock();
	printf("%.2f ms\n", (double)(end - begin));

		imshow("result", result);
		
		


		

		warpPerspective(result, result_LC, H_LC, Size(4 * src_L.cols - width, src_L.rows));//Size设置结果图像宽度，宽度裁去一部分，e可调
		//imshow("result_LC", result_LC);
		Mat half_LC(result_LC, Rect(0, 0, src_L.cols - width, src_L.rows));
		//imshow("half-LC", half_LC);
		src_L(Range::all(), Range(0, src_L.cols - width)).copyTo(half_LC);
		//imshow("half2_LC", half_LC);
		for (int i = 0; i < width; i++)
		{
			result_LC.col(src_L.cols - width + i) = (width - i) / (float)width*src_L.col(src_L.cols - width + i) + i / (float)width*result_LC.col(src_L.cols - width + i);  //权重
		}

		

		//imshow("stitch", result);
		//imshow("stitch", result_LC);
		
		
		waitKey(20);
	//}

		**************************************************************************/

	
}