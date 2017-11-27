/*************************************************
ModuleName:FrechetDistance
Copyright:QZStech
Author:chengmo
E-mail：cmchengmo@163.com
Date:2017-11-21
Description:calc the Frechet Distance (3D)
**************************************************/
弗雷歇距离C++实现
适用于两条三维路径的相似度比较，返回结果为相似率
核心部分可用两种方式解决：A*寻路算法/Bwlabel连通区域算法
相似率计算公式为：probablity = (1-normcdf((result-f_min)/(f_max/3)))*2 
result为弗雷歇距离，f_min为最小间距，f_max为最大间距。