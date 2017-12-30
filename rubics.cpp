#include<iostream>
#include<string>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<cmath>
#include<Windows.h>
#include<WinUser.h>
#include<algorithm>
#include<conio.h> 
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)


using namespace std;
using namespace cv;

string presteps[100000];
string actualsteps[100000];
int num;
int countstep;

struct opera
{
	int opnum;
	bool opcode;
};

struct point
{
	double x;
	double y;
};

struct addpoint
{
	Point add[3][16];
};

struct corner
{
	char cor[8][3];
};

struct edge
{
	char ed[12][2];
};


double dprod(double T1[3], double T2[3])
{
	double ans=0;
	for(int i=0;i<3;i++)
	{
		ans=T1[i]*T2[i]+ans;
	}
	return ans;
}





Point3d gtpoint(double axangle,double axanglen,double spangle,double x,double y,double z)
{
	Point3d ans;
	double x1=x;
	double y1=y*cos(axangle)+z*sin(axangle);
	double z1=-y*sin(axangle)+z*cos(axangle);
	double xn=-sin(axanglen)*y1+cos(axanglen)*x1;
	double yn=cos(axanglen)*y1+sin(axanglen)*x1;
	double zn=z1;
	double x2=xn*cos(spangle)-zn*sin(spangle);
	double y2=yn;
	double z2=xn*sin(spangle)+zn*cos(spangle);
	double xu=sin(axanglen)*y2+cos(axanglen)*x2;
	double yu=cos(axanglen)*y2-sin(axanglen)*x2;
	double zu=z2;
	ans.x=xu;
	ans.y=yu*cos(axangle)-zu*sin(axangle);
	ans.z=yu*sin(axangle)+zu*cos(axangle);
	return ans;

}
int maxget(double arr[8])
{
	int ans=0;
	double jilu=arr[0];
	for(int i=1;i<8;i++)
	{
		if(arr[i]>jilu) 
		{
			ans=i;
			jilu=arr[i];
		}
	}
    return ans;
}

/*addpoint getad(char type,int imsize,Point3d testp[8],int surface[6][4],int draw[3])
{
	addpoint adpoint;
	if (type=='u')
	{
		for(int i=0;i<3;i++)
		{
			if(draw[i]==0)
			{
			for(int j=0;j<4;j++)
					{
						adpoint.add[i][j].x=(testp[surface[draw[i]][0]].y*(3-j)/3+testp[surface[draw[i]][1]].y*j/3)*(imsize/5)+(imsize/2);
						adpoint.add[i][j].y=-(testp[surface[draw[i]][0]].z*(3-j)/3+testp[surface[draw[i]][1]].z*j/3)*(imsize/5)+(imsize/2);
						adpoint.add[i][j+12].x=(testp[surface[draw[i]][3]].y*(3-j)/3+testp[surface[draw[i]][2]].y*j/3)*(imsize/5)+(imsize/2);
						adpoint.add[i][j+12].y=-(testp[surface[draw[i]][3]].z*(3-j)/3+testp[surface[draw[i]][2]].z*j/3)*(imsize/5)+(imsize/2);
					}
					adpoint.add[i][4].x=(testp[surface[draw[i]][0]].y*2/3+testp[surface[draw[i]][3]].y/3)*(imsize/5)+(imsize/2);
					adpoint.add[i][4].y=-(testp[surface[draw[i]][0]].z*2/3+testp[surface[draw[i]][3]].z/3)*(imsize/5)+(imsize/2);
					adpoint.add[i][8].x=(testp[surface[draw[i]][0]].y/3+testp[surface[draw[i]][3]].y*2/3)*(imsize/5)+(imsize/2);
					adpoint.add[i][8].y=-(testp[surface[draw[i]][0]].z/3+testp[surface[draw[i]][3]].z*2/3)*(imsize/5)+(imsize/2);
					adpoint.add[i][7].x=(testp[surface[draw[i]][1]].y*2/3+testp[surface[draw[i]][2]].y/3)*(imsize/5)+(imsize/2);
					adpoint.add[i][7].y=-(testp[surface[draw[i]][1]].z*2/3+testp[surface[draw[i]][2]].z/3)*(imsize/5)+(imsize/2);
					adpoint.add[i][11].x=(testp[surface[draw[i]][1]].y/3+testp[surface[draw[i]][2]].y*2/3)*(imsize/5)+(imsize/2);
					adpoint.add[i][11].y=-(testp[surface[draw[i]][1]].z/3+testp[surface[draw[i]][2]].z*2/3)*(imsize/5)+(imsize/2);
					adpoint.add[i][5].x=adpoint.add[i][4].x*2/3+adpoint.add[i][7].x*1/3;
					adpoint.add[i][5].y=adpoint.add[i][4].y*2/3+adpoint.add[i][7].y*1/3;
					adpoint.add[i][6].x=adpoint.add[i][4].x*1/3+adpoint.add[i][7].x*2/3;
					adpoint.add[i][6].y=adpoint.add[i][4].y*1/3+adpoint.add[i][7].y*2/3;
					adpoint.add[i][9].x=adpoint.add[i][8].x*2/3+adpoint.add[i][11].x*1/3;
					adpoint.add[i][9].y=adpoint.add[i][8].y*2/3+adpoint.add[i][11].y*1/3;
					adpoint.add[i][10].x=adpoint.add[i][8].x*1/3+adpoint.add[i][11].x*2/3;
					adpoint.add[i][10].y=adpoint.add[i][8].y*1/3+adpoint.add[i][11].y*2/3;
	        }
			else if(draw[i]==1)
			{
					for(int j=0;j<4;j++)
					{
						adpoint.add[i][j].x=(testp[surface[draw[i]][0]].y*(3-j)/3+testp[surface[draw[i]][1]].y*j/3)*(imsize/5)+(imsize/2);
						adpoint.add[i][j].y=-(testp[surface[draw[i]][0]].z*(3-j)/3+testp[surface[draw[i]][1]].z*j/3)*(imsize/5)+(imsize/2);
						adpoint.add[i][j+12].x=(testp[surface[draw[i]][3]].y*(3-j)/3+testp[surface[draw[i]][2]].y*j/3)*(imsize/5)+(imsize/2);
						adpoint.add[i][j+12].y=-(testp[surface[draw[i]][3]].z*(3-j)/3+testp[surface[draw[i]][2]].z*j/3)*(imsize/5)+(imsize/2);
					}
			}
	 }
	
	{
		
		for(int j=0;j<4;j++)
		{
			adpoint.add[j].x=(testp[surface[draw[i]][0]].y*(3-j)/3+testp[surface[draw[i]][1]].y*j/3)*(imsize/5)+(imsize/2);
			adpoint.add[j].y=-(testp[surface[draw[i]][0]].z*(3-j)/3+testp[surface[draw[i]][1]].z*j/3)*(imsize/5)+(imsize/2);
	    }
		adpoint
	}


}*/

void DrawCube(Mat screen,int imsize,Point3d testp[8],int surface[6][4],Scalar surfc[26],char rubic[6][10])
{
			Point Dpoint[1][4];
			double judge[8];
			for(int i=0;i<8;i++)
			{
				judge[i]=testp[i].x;
			}
			int front=maxget(judge);
			int draw[3];
			int count=0;
			for(int i=0;i<6;i++)
			{
				for(int j=0;j<4;j++)
				{
					if (surface[i][j]==front)
						{
							draw[count]=i;
							count++;
							break;
					    }
				}

			}

			for(int i=0;i<count;i++)
			{
				Point addpoint[16];
				Point Dpoint[1][4];
				for(int j=0;j<4;j++)
				{
					addpoint[j].x=(testp[surface[draw[i]][0]].y*(3-j)/3+testp[surface[draw[i]][1]].y*j/3)*(imsize/5)+(imsize/2);
					addpoint[j].y=-(testp[surface[draw[i]][0]].z*(3-j)/3+testp[surface[draw[i]][1]].z*j/3)*(imsize/5)+(imsize/2);
					addpoint[j+12].x=(testp[surface[draw[i]][3]].y*(3-j)/3+testp[surface[draw[i]][2]].y*j/3)*(imsize/5)+(imsize/2);
					addpoint[j+12].y=-(testp[surface[draw[i]][3]].z*(3-j)/3+testp[surface[draw[i]][2]].z*j/3)*(imsize/5)+(imsize/2);
				}
				addpoint[4].x=(testp[surface[draw[i]][0]].y*2/3+testp[surface[draw[i]][3]].y/3)*(imsize/5)+(imsize/2);
				addpoint[4].y=-(testp[surface[draw[i]][0]].z*2/3+testp[surface[draw[i]][3]].z/3)*(imsize/5)+(imsize/2);
				addpoint[8].x=(testp[surface[draw[i]][0]].y/3+testp[surface[draw[i]][3]].y*2/3)*(imsize/5)+(imsize/2);
				addpoint[8].y=-(testp[surface[draw[i]][0]].z/3+testp[surface[draw[i]][3]].z*2/3)*(imsize/5)+(imsize/2);
				addpoint[7].x=(testp[surface[draw[i]][1]].y*2/3+testp[surface[draw[i]][2]].y/3)*(imsize/5)+(imsize/2);
				addpoint[7].y=-(testp[surface[draw[i]][1]].z*2/3+testp[surface[draw[i]][2]].z/3)*(imsize/5)+(imsize/2);
				addpoint[11].x=(testp[surface[draw[i]][1]].y/3+testp[surface[draw[i]][2]].y*2/3)*(imsize/5)+(imsize/2);
				addpoint[11].y=-(testp[surface[draw[i]][1]].z/3+testp[surface[draw[i]][2]].z*2/3)*(imsize/5)+(imsize/2);
				addpoint[5].x=addpoint[4].x*2/3+addpoint[7].x*1/3;
				addpoint[5].y=addpoint[4].y*2/3+addpoint[7].y*1/3;
				addpoint[6].x=addpoint[4].x*1/3+addpoint[7].x*2/3;
				addpoint[6].y=addpoint[4].y*1/3+addpoint[7].y*2/3;
				addpoint[9].x=addpoint[8].x*2/3+addpoint[11].x*1/3;
				addpoint[9].y=addpoint[8].y*2/3+addpoint[11].y*1/3;
				addpoint[10].x=addpoint[8].x*1/3+addpoint[11].x*2/3;
				addpoint[10].y=addpoint[8].y*1/3+addpoint[11].y*2/3;

				int countc=0;
				for(int j=0;j<11;j++)
				{
					if((j==3)||(j==7)) continue;
				Dpoint[0][0].x=addpoint[j].x;
				Dpoint[0][0].y=addpoint[j].y;
				Dpoint[0][1].x=addpoint[j+1].x;
				Dpoint[0][1].y=addpoint[j+1].y;
				Dpoint[0][2].x=addpoint[j+5].x;
				Dpoint[0][2].y=addpoint[j+5].y;
				Dpoint[0][3].x=addpoint[j+4].x;
				Dpoint[0][3].y=addpoint[j+4].y;

               const Point* pt[1] = { Dpoint[0] };
			   int npt[1] = {4};
			   fillPoly(screen, pt, npt, 1,surfc[rubic[draw[i]][countc]-'a'], 8);
			   polylines(screen, pt, npt, 1, 1, Scalar(0,0,0)); 
			   countc++;
				}
				
			}

			imshow("Rubic's_Cube",screen);
			return;

}


void op0(char rubic[6][10],char RUB[6][10],int opnum)
{
	rubic[opnum][8]=RUB[opnum][6];
	rubic[opnum][5]=RUB[opnum][7];
	rubic[opnum][2]=RUB[opnum][8];
	rubic[opnum][1]=RUB[opnum][5];
	rubic[opnum][0]=RUB[opnum][2];
	rubic[opnum][3]=RUB[opnum][1];
	rubic[opnum][6]=RUB[opnum][0];
	rubic[opnum][7]=RUB[opnum][3];
}

void op1(char rubic[6][10],char RUB[6][10],int opnum)
{
	rubic[opnum][8]=RUB[opnum][2];
	rubic[opnum][7]=RUB[opnum][5];
	rubic[opnum][6]=RUB[opnum][8];
	rubic[opnum][3]=RUB[opnum][7];
	rubic[opnum][0]=RUB[opnum][6];
	rubic[opnum][1]=RUB[opnum][3];
	rubic[opnum][2]=RUB[opnum][0];
	rubic[opnum][5]=RUB[opnum][1];
}


void U(char rubic[6][10])
{
	presteps[countstep]="U";
	countstep++;
	char RUB[6][10];
	for(int i=0;i<6;i++)
	{
		for(int j=0;j<10;j++)
		{
		RUB[i][j]=rubic[i][j];
		}
	}
	op0(rubic,RUB,0);
	for(int i=0;i<3;i++)
	{
		rubic[2][i]=RUB[5][i];
		rubic[5][i]=RUB[3][2-i];
		rubic[3][i]=RUB[4][i];
		rubic[4][i]=RUB[2][2-i];
	}
}

void Ui(char rubic[6][10])
{
	U(rubic);
	U(rubic);
	U(rubic);
}

void D(char rubic[6][10])
{
	presteps[countstep]="D";
	countstep++;
	char RUB[6][10];
	for(int i=0;i<6;i++)
	{
		for(int j=0;j<10;j++)
		{
		RUB[i][j]=rubic[i][j];
		}
	}
	op1(rubic,RUB,1);
	for(int i=6;i<9;i++)
	{
		rubic[2][i]=RUB[4][14-i];
		rubic[4][i]=RUB[3][i];
		rubic[3][i]=RUB[5][14-i];
		rubic[5][i]=RUB[2][i];
	}
}

void Di(char rubic[6][10])
{

	D(rubic);
	D(rubic);
	D(rubic);
}

void F(char rubic[6][10])
{
	presteps[countstep]="F";
	countstep++;
	char RUB[6][10];
	for(int i=0;i<6;i++)
	{
		for(int j=0;j<10;j++)
		{
		RUB[i][j]=rubic[i][j];
		}
	}
	op0(rubic,RUB,2);
	int i=0;
	rubic[0][i]=RUB[4][i+2];
	rubic[1][i]=RUB[5][i+2];
	rubic[5][8-i]=RUB[0][i];
	rubic[4][8-i]=RUB[1][i];
	i=3;
	rubic[0][i]=RUB[4][i+2];
	rubic[1][i]=RUB[5][i+2];
	rubic[5][8-i]=RUB[0][i];
	rubic[4][8-i]=RUB[1][i];
	i=6;
	rubic[0][i]=RUB[4][i+2];
	rubic[1][i]=RUB[5][i+2];
	rubic[5][8-i]=RUB[0][i];
	rubic[4][8-i]=RUB[1][i];
}

void Fi(char rubic[6][10])
{

	F(rubic);
	F(rubic);
	F(rubic);
}

void B(char rubic[6][10])
{
	presteps[countstep]="B";
	countstep++;
	char RUB[6][10];
	for(int i=0;i<6;i++)
	{
		for(int j=0;j<10;j++)
		{
		RUB[i][j]=rubic[i][j];
		}
	}
	op1(rubic,RUB,3);
	int i=2;
	rubic[0][i]=RUB[5][8-i];
	rubic[1][i]=RUB[4][8-i];
	rubic[4][8-i]=RUB[0][10-i];
	rubic[5][8-i]=RUB[1][10-i];
	i=5;
    rubic[0][i]=RUB[5][8-i];
	rubic[1][i]=RUB[4][8-i];
	rubic[4][8-i]=RUB[0][10-i];
	rubic[5][8-i]=RUB[1][10-i];
    i=8;
    rubic[0][i]=RUB[5][8-i];
	rubic[1][i]=RUB[4][8-i];
	rubic[4][8-i]=RUB[0][10-i];
	rubic[5][8-i]=RUB[1][10-i];
}

void Bi(char rubic[6][10])
{

	B(rubic);
	B(rubic);
	B(rubic);
}

void L(char rubic[6][10])
{
	presteps[countstep]="L";
	countstep++;
	char RUB[6][10];
	for(int i=0;i<6;i++)
	{
		for(int j=0;j<10;j++)
		{
		RUB[i][j]=rubic[i][j];
		}
	}
	op1(rubic,RUB,4);
	rubic[0][6]=RUB[3][2];
	rubic[0][7]=RUB[3][5];
	rubic[0][8]=RUB[3][8];
	rubic[1][6]=RUB[2][2];
	rubic[1][7]=RUB[2][5];
	rubic[1][8]=RUB[2][8];
	rubic[3][2]=RUB[1][8];
	rubic[3][5]=RUB[1][7];
	rubic[3][8]=RUB[1][6];
	rubic[2][2]=RUB[0][8];
	rubic[2][5]=RUB[0][7];
	rubic[2][8]=RUB[0][6];
}

void Li(char rubic[6][10])
{
	L(rubic);
	L(rubic);
	L(rubic);
}

void R(char rubic[6][10])
{
	presteps[countstep]="R";
	countstep++;
	char RUB[6][10];
	for(int i=0;i<6;i++)
	{
		for(int j=0;j<10;j++)
		{
		RUB[i][j]=rubic[i][j];
		}
	}
	op0(rubic,RUB,5);
	rubic[0][2]=RUB[2][0];
	rubic[0][1]=RUB[2][3];
	rubic[0][0]=RUB[2][6];
	rubic[1][2]=RUB[3][0];
	rubic[1][1]=RUB[3][3];
	rubic[1][0]=RUB[3][6];
	rubic[2][0]=RUB[1][0];
	rubic[2][3]=RUB[1][1];
	rubic[2][6]=RUB[1][2];
	rubic[3][0]=RUB[0][0];
	rubic[3][3]=RUB[0][1];
	rubic[3][6]=RUB[0][2];
}

void Ri(char rubic[6][10])
{
	R(rubic);
	R(rubic);
	R(rubic);
}





corner getcorner(char rubic[6][10])
{
	corner ans;
	ans.cor[0][0]=rubic[2][0];
	ans.cor[0][1]=rubic[5][2];
	ans.cor[0][2]=rubic[0][0];
	ans.cor[1][0]=rubic[5][0];
	ans.cor[1][1]=rubic[3][0];
	ans.cor[1][2]=rubic[0][2];
	ans.cor[2][0]=rubic[3][2];
	ans.cor[2][1]=rubic[4][0];
	ans.cor[2][2]=rubic[0][8];
	ans.cor[3][0]=rubic[4][2];
	ans.cor[3][1]=rubic[2][2];
	ans.cor[3][2]=rubic[0][6];
	ans.cor[4][0]=rubic[2][6];
	ans.cor[4][1]=rubic[5][8];
	ans.cor[4][2]=rubic[1][0];
	ans.cor[5][0]=rubic[5][6];
	ans.cor[5][1]=rubic[3][6];
	ans.cor[5][2]=rubic[1][2];
	ans.cor[6][0]=rubic[3][8];
	ans.cor[6][1]=rubic[4][6];
	ans.cor[6][2]=rubic[1][8];
	ans.cor[7][0]=rubic[4][8];
	ans.cor[7][1]=rubic[2][8];
	ans.cor[7][2]=rubic[1][6];
	return ans;
}
edge getedge (char rubic[6][10])
{
	edge ans;
	ans.ed[0][0]=rubic[2][1];
	ans.ed[0][1]=rubic[0][3];
	ans.ed[1][0]=rubic[5][1];
	ans.ed[1][1]=rubic[0][1];
	ans.ed[2][0]=rubic[3][1];
	ans.ed[2][1]=rubic[0][5];
	ans.ed[3][0]=rubic[4][1];
	ans.ed[3][1]=rubic[0][7];
	ans.ed[4][0]=rubic[2][3];
	ans.ed[4][1]=rubic[5][5];
	ans.ed[5][0]=rubic[5][3];
	ans.ed[5][1]=rubic[3][3];
	ans.ed[6][0]=rubic[3][5];
	ans.ed[6][1]=rubic[4][3];
	ans.ed[7][0]=rubic[4][5];
	ans.ed[7][1]=rubic[2][5];
	ans.ed[8][0]=rubic[2][7];
	ans.ed[8][1]=rubic[1][3];
	ans.ed[9][0]=rubic[5][7];
	ans.ed[9][1]=rubic[1][1];
	ans.ed[10][0]=rubic[3][7];
	ans.ed[10][1]=rubic[1][5];
	ans.ed[11][0]=rubic[4][7];
	ans.ed[11][1]=rubic[1][7];
	return ans;
}


void opstep1 (int tem,char rubic[6][10])
{
	switch(tem)
	{
		case 4:U(rubic);break;
		case 5:D(rubic);break;
		case 0:F(rubic);break;
		case 2:B(rubic);break;
		case 3:L(rubic);break;
		case 1:R(rubic);break;
		case 10:Ui(rubic);break;
		case 11:Di(rubic);break;
		case 6:Fi(rubic);break;
		case 8:Bi(rubic);break;
		case 9:Li(rubic);break;
		case 7:Ri(rubic);break;
		default:break;
	}
}

void optest(string tem,char rubic[6][10])
{
	if(tem=="U")U(rubic);
	if(tem=="D")D(rubic);
	if(tem=="F")F(rubic);
	if(tem=="B")B(rubic);
	if(tem=="L")L(rubic);
	if(tem=="R")R(rubic);
	if(tem=="Ui")Ui(rubic);
	if(tem=="Di")Di(rubic);
	if(tem=="Fi")Fi(rubic);
	if(tem=="Bi")Bi(rubic);
	if(tem=="Li")Li(rubic);
	if(tem=="Ri")Ri(rubic);
}

//step0:顶面小花//
void step0(char rubic[6][10])
{
    
	bool judge=0;
	edge tem1=getedge(rubic);
	if ((tem1.ed[0][1]=='w')&&(tem1.ed[1][1]=='w')&&(tem1.ed[2][1]=='w')&&(tem1.ed[3][1]=='w')) judge=1;
	while(judge==0)
	{
		for(int i=0;i<4;i++)
		{
			if(tem1.ed[i][0]=='w')
			{
				opstep1(i,rubic);
				tem1=getedge(rubic);
				while(tem1.ed[(i+1)%4][1]=='w')
				{
				opstep1(4,rubic);
				tem1=getedge(rubic);
				}
				opstep1((i+1)%4,rubic);
				tem1=getedge(rubic);
			}
		}

		for(int i=4;i<8;i++)
		{
			if(tem1.ed[i][0]=='w')
			{
				while(tem1.ed[(i+1)%4][1]=='w')
				{
					opstep1(4,rubic);
					tem1=getedge(rubic);
				}
				opstep1((i+1)%4,rubic);
				tem1=getedge(rubic);
			}
			if(tem1.ed[i][1]=='w')
			{
				while(tem1.ed[(i-1)%4][1]=='w')
				{
					opstep1(4,rubic);
					tem1=getedge(rubic);
				}
				opstep1((i%4)+6,rubic);
				tem1=getedge(rubic);
			}
		}
		for(int i=8;i<12;i++)
		{
			if(tem1.ed[i][0]=='w')
			{
				while(tem1.ed[i%4][1]=='w')
				{
					opstep1(4,rubic);
					tem1=getedge(rubic);
				}
				opstep1((i%4)+6,rubic);
				tem1=getedge(rubic);
				while(tem1.ed[(i+1)%4][1]=='w')
				{
					opstep1(4,rubic);
					tem1=getedge(rubic);
				}
				opstep1((i+1)%4,rubic);
				tem1=getedge(rubic);
			}

			if(tem1.ed[i][1]=='w')
			{
				while(tem1.ed[i%4][1]=='w')
				{
					opstep1(4,rubic);
					tem1=getedge(rubic);
				}
				opstep1((i%4),rubic);
				tem1=getedge(rubic);
				opstep1((i%4),rubic);
				tem1=getedge(rubic);
			}
		}
		if ((tem1.ed[0][1]=='w')&&(tem1.ed[1][1]=='w')&&(tem1.ed[2][1]=='w')&&(tem1.ed[3][1]=='w')) judge=1;
	}
	return;
}
//step1：底面十字//
void step1(char rubic[6][10])
{
	
	edge tem1=getedge(rubic);
	char col[4]={'b','r','g','o'};
	if ((tem1.ed[0][1]=='w')&&(tem1.ed[1][1]=='w')&&(tem1.ed[2][1]=='w')&&(tem1.ed[3][1]=='w'))
	{
		for(int i=0;i<4;i++)
		{
			while((tem1.ed[i][1]!='w')||(tem1.ed[i][0]!=col[i]))
			{
				opstep1(4,rubic);
			    tem1=getedge(rubic);
			}
			opstep1(i,rubic);
			opstep1(i,rubic);
			tem1=getedge(rubic);
		}
	}
	else
	{
		cout<<"step 0 Undone!"<<endl;
		return;
	}
	return;
}
//step2：底面+一层//
void step2 (char rubic[6][10])
{
	
	char col[4]={'b','r','g','o'};
	corner tem1=getcorner(rubic);
	edge tem2=getedge(rubic);
	int judge=1;
	for(int i=0;i<4;i++)
	{
		if((tem2.ed[i+8][1]!='w')||(tem2.ed[i+8][0]!=col[i]))
		{
			judge=0;
			break;
		}
	}
	if (judge==0)
	{
		cout<<"Step 1 Undone!"<<endl;
		return;
	}
	for(int i=0;i<4;i++)
	{
		if ((tem1.cor[i+4][0]!=col[i])||(tem1.cor[i+4][1]!=col[(i+1)%4])||(tem2.ed[i+8][0]!=col[i]))
		{
			judge=0;
			break;
		}

	}

	while(judge==0)
	{
			for(int i=0;i<4;i++)
			{
				if(tem1.cor[i][0]=='w')
				{
					int turns;
					int j;
					for(j=0;j<4;j++)
					{
						if (col[j]==tem1.cor[i][1]) 
						{
							turns=(j-i-1+8)%4;
							break;
						}
					}
					for(int m=0;m<turns;m++) opstep1(10,rubic);
					opstep1((j+3)%4+6,rubic);
					opstep1(10,rubic);
					opstep1((j+3)%4,rubic);
					tem1=getcorner(rubic);
				}

				if(tem1.cor[i][1]=='w')
				{
					int turns;
					int j;
					for(j=0;j<4;j++)
					{
						if(col[j]==tem1.cor[i][0])
						{
							turns=(j-i+8)%4;
							break;
						}
					}
					for(int m=0;m<turns;m++) opstep1(10,rubic);
					opstep1((j+1)%4,rubic);
					opstep1(4,rubic);
					opstep1((j+1)%4+6,rubic);
					tem1=getcorner(rubic);
				}
			}

			for(int i=0;i<4;i++)
			{
				if(tem1.cor[i][2]=='w')
				{
					int turns=0;
					int j;
					for(j=0;j<4;j++)
					{
						if((tem1.cor[i][0]==col[(j+1)%4])&&(tem1.cor[i][1]==col[j]))
						{
							turns=(j-i+8)%4;
							break;
						}
					}
					for(int m=0;m<turns;m++) opstep1(10,rubic);
					opstep1(j+6,rubic);
					opstep1(10,rubic);
					opstep1(10,rubic);
					opstep1(j,rubic);
					opstep1(4,rubic);
					opstep1(j+6,rubic);
					opstep1(10,rubic);
					opstep1(j,rubic);
					tem1=getcorner(rubic);
				}
			}

			for(int i=0;i<4;i++)
			{
				if(tem1.cor[i+4][0]=='w')
				{
					if((tem1.cor[i][0]=='w')||(tem1.cor[i][1]=='w')||(tem1.cor[i][2]=='w')) opstep1(10,rubic);
					opstep1(i+6,rubic);
					opstep1(10,rubic);
					opstep1(i,rubic);
					opstep1(4,rubic);
					tem1=getcorner(rubic);
					int turns;
					int j;
					for(j=0;j<4;j++)
					{
						if (col[j]==tem1.cor[i][1]) 
						{
							turns=(j-i-1+8)%4;
							break;
						}
					}
					for(int m=0;m<turns;m++) opstep1(10,rubic);
					opstep1((j+3)%4+6,rubic);
					opstep1(10,rubic);
					opstep1((j+3)%4,rubic);
					tem1=getcorner(rubic);
				}

				if(tem1.cor[i+4][1]=='w')
				{
					if((tem1.cor[i][0]=='w')||(tem1.cor[i][1]=='w')||(tem1.cor[i][2]=='w')) opstep1(4,rubic);
					opstep1((i+1)%4,rubic);
					opstep1(4,rubic);
					opstep1((i+1)%4+6,rubic);
					opstep1(10,rubic);
					tem1=getcorner(rubic);
					int turns;
					int j;
					for(j=0;j<4;j++)
					{
						if(col[j]==tem1.cor[i][0])
						{
							turns=(j-i)%4;
							break;
						}
					}
					for(int m=0;m<turns;m++) opstep1(10,rubic);
					opstep1((j+1)%4,rubic);
					opstep1(4,rubic);
					opstep1((j+1)%4+6,rubic);
					tem1=getcorner(rubic);
				}

				if((tem1.cor[i+4][2]=='w')&&((tem1.cor[i+4][0]!=col[i])||(tem1.cor[i+4][1]!=col[(i+1)%4])))
				{
					if((tem1.cor[i][0]=='w')||(tem1.cor[i][1]=='w')||(tem1.cor[i][2]=='w')) opstep1(10,rubic);
					opstep1(i+6,rubic);
					opstep1(10,rubic);
					opstep1(i,rubic);
					opstep1(4,rubic);
					tem1=getcorner(rubic);
					int turns;
					int j;
					for(j=0;j<4;j++)
					{
						if(col[j]==tem1.cor[i][0])
						{
							turns=(j-i)%4;
							break;
						}
					}
					for(int m=0;m<turns;m++) opstep1(10,rubic);
					opstep1((j+1)%4,rubic);
					opstep1(4,rubic);
					opstep1((j+1)%4+6,rubic);
					tem1=getcorner(rubic);
				}
	         }

			judge=1;
			tem2=getedge(rubic);
			for(int i=0;i<4;i++)
			{
				if ((tem1.cor[i+4][0]!=col[i])||(tem1.cor[i+4][1]!=col[(i+1)%4])||(tem2.ed[i+8][0]!=col[i]))
				{
					judge=0;
					break;
				}
			}
		}
	}
//step3：二层//
void step3(char rubic[6][10])
{
	char col[4]={'b','r','g','o'};
	edge tem1=getedge(rubic);
	corner tem2=getcorner(rubic);
	int judge=1;
	for(int i=0;i<4;i++)
	{
		if ((tem2.cor[i+4][0]!=col[i])||(tem2.cor[i+4][1]!=col[(i+1)%4])||(tem1.ed[i+8][0]!=col[i]))
		{
			judge=0;
			break;
		}
	}
	if(judge==0)
	{
		cout<<"Step 2 Undone!"<<endl;
		return;
	}

	for(int i=0;i<4;i++)
	{
		if((tem1.ed[i+4][0]!=col[i])||(tem1.ed[i+4][1]!=col[(i+1)%4]))
		{
			judge=0;
			break;
		}
	}

	while(judge==0)
	{
		for(int i=0;i<4;i++)
		{
			if((tem1.ed[i][0]!='y')&&(tem1.ed[i][1]!='y'))
			{
				int turns;
				int j;
				for(j=0;j<4;j++)
				{
					if(tem1.ed[i][0]==col[j])
					{
						turns=(j-i+8)%4;
						break;
					}
				}
				for(int p=0;p<turns;p++) opstep1(10,rubic);
				if (col[(j+1)%4]==tem1.ed[i][1])
				{
					opstep1(4,rubic);
					opstep1((j+1)%4,rubic);
					opstep1(10,rubic);
					opstep1((j+1)%4+6,rubic);
					opstep1(10,rubic);
					opstep1(j+6,rubic);
					opstep1(4,rubic);
					opstep1(j,rubic);
					tem1=getedge(rubic);
				}
				else
				{
					opstep1(10,rubic);
					opstep1((j+3)%4+6,rubic);
					opstep1(4,rubic);
					opstep1((j+3)%4,rubic);
					opstep1(4,rubic);
					opstep1(j,rubic);
					opstep1(10,rubic);
					opstep1(j+6,rubic);
					tem1=getedge(rubic);
				}
			}
		}
		for(int i=0;i<4;i++)
		{
			if((tem1.ed[i+4][0]!=col[i])||(tem1.ed[i+4][1]!=col[(i+1)%4]))
			{
				while((tem1.ed[i][0]!='y')&&(tem1.ed[i][1]!='y'))
				{
					opstep1(4,rubic);
					tem1=getedge(rubic);
				}
				opstep1(4,rubic);
				opstep1((i+1)%4,rubic);
				opstep1(10,rubic);
				opstep1((i+1)%4+6,rubic);
				opstep1(10,rubic);
				opstep1(i+6,rubic);
				opstep1(4,rubic);
				opstep1(i,rubic);
				tem1=getedge(rubic);
			}
		}
		judge=1;
		for(int i=0;i<4;i++)
		{
			if((tem1.ed[i+4][0]!=col[i])||(tem1.ed[i+4][1]!=col[(i+1)%4]))
			{
				judge=0;
				break;
			}
		}
	}

}
//step4：顶面十字//
void step4(char rubic[6][10])
{
	edge tem1=getedge(rubic);
	char col[4]={'b','r','g','o'};
	int judge=1;
	for(int i=0;i<4;i++)
	{
		if((tem1.ed[i+4][0]!=col[i])||(tem1.ed[i+4][1]!=col[(i+1)%4]))
		{
			judge=0;
			break;
		}
	}
	if(judge==0)
	{
		cout<<"Step 3 Undone!"<<endl;
		return;
	}
	int stage=0;
	for(int i=0;i<4;i++)
		{
			if(tem1.ed[i][1]=='y')
			{
				if(tem1.ed[(i+2)%4][1]=='y')
				{
					if(stage<2) stage=2;
					if((tem1.ed[(i+1)%4][1]=='y')&&(tem1.ed[(i+3)%4][1]=='y'))
					{
						if(stage<3) stage=3;
					}
				}
				 else
				{
					if((tem1.ed[(i+1)%4][1]=='y')||(tem1.ed[(i+3)%4][1]=='y'))
					{
							if(stage<1) stage=1;
					}
				}
			}
		}

	while(stage!=3)
	{
		cout<<stage<<endl;
		switch(stage)
		{
		case 0:
			{
				opstep1(0,rubic);
				opstep1(1,rubic);
				opstep1(4,rubic);
				opstep1(7,rubic);
				opstep1(10,rubic);
				opstep1(6,rubic);
				tem1=getedge(rubic);
			}
		case 1:
			{
				for(int i=0;i<4;i++)
				{
					if((tem1.ed[i][1]=='y')&&(tem1.ed[(i+1)%4][1]=='y'))
					{
						opstep1(i,rubic);
						opstep1((i+1)%4,rubic);
						opstep1(4,rubic);
						opstep1((i+1)%4+6,rubic);
						opstep1(10,rubic);
						opstep1(i+6,rubic);
						tem1=getedge(rubic);
						break;
					}
				}
			}
		case 2:
			{
				for(int i=0;i<4;i++)
				{
					if((tem1.ed[(i+3)%4][1]=='y')&&(tem1.ed[(i+1)%4][1]=='y'))
					{
						opstep1(i,rubic);
						opstep1((i+1)%4,rubic);
						opstep1(4,rubic);
						opstep1((i+1)%4+6,rubic);
						opstep1(10,rubic);
						opstep1(i+6,rubic);
						tem1=getedge(rubic);
						break;
					}
				}
			}
		}

		for(int i=0;i<4;i++)
		{
			if(tem1.ed[i][1]=='y')
			{
				if(tem1.ed[(i+2)%4][1]=='y')
				{
					if(stage<2) stage=2;
					if((tem1.ed[(i+1)%4][1]=='y')&&(tem1.ed[(i+3)%4][1]=='y'))
					{
						if(stage<3) stage=3;
					}
				}
				 else
				{
					if((tem1.ed[(i+1)%4][1]=='y')||(tem1.ed[(i+3)%4][1]=='y'))
					{
							if(stage<1) stage=1;
					}
				}
			}
		}
	}
	
	return;
}
//step5：顶面//
void step5(char rubic[6][10])
{
	corner tem1=getcorner(rubic);
	int judge=0;
	for(int i=0;i<4;i++)
	{
		if((tem1.cor[i][2]=='y')&&(tem1.cor[(i+1)%4][2]!='y')&&(tem1.cor[(i+2)%4][2]!='y')&&(tem1.cor[(i+3)%4][2]!='y')) 
		{
			judge=1;
			break;
		}
	}
	int count=0;
	while(judge==0)
	{
		if(count%6==0) opstep1(4,rubic);
		opstep1(3,rubic);
		opstep1(4,rubic);
		opstep1(9,rubic);
		opstep1(4,rubic);
		opstep1(3,rubic);
		opstep1(4,rubic);
		opstep1(4,rubic);
		opstep1(9,rubic);
		tem1=getcorner(rubic);
		for(int i=0;i<4;i++)
		{
			if((tem1.cor[i][2]=='y')&&(tem1.cor[(i+1)%4][2]!='y')&&(tem1.cor[(i+2)%4][2]!='y')&&(tem1.cor[(i+3)%4][2]!='y')) 
			{
				judge=1;
				break;
			}
		}
		count++;
	}
	bool face=1;
	for(int i=0;i<4;i++)
	{
		if(tem1.cor[i][2]=='y')
		{
			if(tem1.cor[(i+2)%4][0]=='y') 
				{
					face=0;
					break;
		     	}
			else break;
		}
	}
	if(face==0)
	{
		while(tem1.cor[1][2]!='y')
		{
			opstep1(4,rubic);
			tem1=getcorner(rubic);
		}
		opstep1(3,rubic);
		opstep1(4,rubic);
		opstep1(9,rubic);
		opstep1(4,rubic);
		opstep1(3,rubic);
		opstep1(4,rubic);
		opstep1(4,rubic);
		opstep1(9,rubic);
	}
	else
	{
		while(tem1.cor[2][2]!='y')
		{
			opstep1(4,rubic);
			tem1=getcorner(rubic);
		}
		opstep1(7,rubic);
		opstep1(10,rubic);
		opstep1(1,rubic);
		opstep1(10,rubic);
		opstep1(7,rubic);
		opstep1(10,rubic);
		opstep1(10,rubic);
		opstep1(1,rubic);
	}


}
//step6：三层//
void step6(char rubic[6][10])
{
	corner tem1=getcorner(rubic);
	bool flag=0;
	char col[4]={'b','r','g','o'};
	bool don=1;
	for(int i=0;i<4;i++)
	{
		if((tem1.cor[i][1]!=tem1.cor[(i+1)%4][0]))
		{
			don=0;
			break;
		}
	}
	if(don==1) return;
	for(int i=0;i<4;i++)
	{
		if (tem1.cor[i][1]==tem1.cor[(i+1)%4][0])
		{
			flag=1;
			int j;
			int turns;
			for(j=0;j<4;j++)
			{
				if (col[j]==tem1.cor[i][1])
				{
					turns=(j-i+7)%4;
					break;
				}
			}
			for(int p=0;p<turns;p++) opstep1(10,rubic);
			opstep1(4,rubic);
			opstep1(j,rubic);
			opstep1((j+1)%4+6,rubic);
			opstep1(j,rubic);
			opstep1((j+3)%4,rubic);
			opstep1((j+3)%4,rubic);
			opstep1(j+6,rubic);
			opstep1((j+1)%4,rubic);
			opstep1(j,rubic);
			opstep1((j+3)%4,rubic);
			opstep1((j+3)%4,rubic);
			opstep1(j,rubic);
			opstep1(j,rubic);
			return;
		}
	}

	if(flag==0)
	{
		int j=0;
		opstep1(j,rubic);
		opstep1((j+1)%4+6,rubic);
		opstep1(j,rubic);
		opstep1((j+3)%4,rubic);
		opstep1((j+3)%4,rubic);
		opstep1(j+6,rubic);
		opstep1((j+1)%4,rubic);
		opstep1(j,rubic);
		opstep1((j+3)%4,rubic);
		opstep1((j+3)%4,rubic);
		opstep1(j,rubic);
		opstep1(j,rubic);
		don=1;
		for(int i=0;i<4;i++)
		{
			if((tem1.cor[i][1]!=tem1.cor[(i+1)%4][0]))
			{
				don=0;
				break;
			}
		}
		if(don==1) return;
		tem1=getcorner(rubic);
		for(int i=0;i<4;i++)
	    {
			if (tem1.cor[i][1]==tem1.cor[(i+1)%4][0])
			{
				int turns;
				for(j=0;j<4;j++)
				{
					if (col[j]==tem1.cor[i][1])
					{
						turns=(j-i+7)%4;
						break;
					}
				}
				for(int p=0;p<turns;p++) opstep1(10,rubic);
				opstep1(4,rubic);
				opstep1(j,rubic);
				opstep1((j+1)%4+6,rubic);
				opstep1(j,rubic);
				opstep1((j+3)%4,rubic);
				opstep1((j+3)%4,rubic);
				opstep1(j+6,rubic);
				opstep1((j+1)%4,rubic);
				opstep1(j,rubic);
				opstep1((j+3)%4,rubic);
				opstep1((j+3)%4,rubic);
				opstep1(j,rubic);
				opstep1(j,rubic);
				return;
			}
	    }
	}
}
//step7：完成//
void step7(char rubic[6][10])
{
	edge tem1=getedge(rubic);
	corner tem2=getcorner(rubic);
	char col[4]={'b','r','g','o'};
	bool don=1;
	bool predon=1;
	while(tem2.cor[0][0]!='b')
	{
		opstep1(4,rubic);
		tem2=getcorner(rubic);
	}
	tem1=getedge(rubic);
	for(int i=0;i<4;i++)
	{
		if((tem1.ed[i][0]!=col[i])||(tem1.ed[i][0]!=tem2.cor[i][0])) don=0;
		if(tem1.ed[i][0]!=tem2.cor[i][0]) predon=0;
	}
	if(don==1) return;
	if(predon==1)
	{
		while(tem1.ed[0][0]!=col[0])
		{
			opstep1(4,rubic);
		}
	return;
	}
	 tem1=getedge(rubic);
	bool flag=0;
	for(int i=0;i<4;i++)
	{
		if(tem1.ed[i][0]==col[i]) 
		{
			flag=1;
			break;
		}
	}
	if(flag==0)
	{
		int i=0;
		opstep1((i+1)%4+6,rubic);
		opstep1(10,rubic);
		opstep1((i+1)%4,rubic);
		opstep1(10,rubic);
		opstep1((i+1)%4+6,rubic);
		opstep1(10,rubic);
		opstep1(10,rubic);
		opstep1((i+1)%4,rubic);
		opstep1(10,rubic);
		opstep1((i+3)%4,rubic);
		opstep1(4,rubic);
		opstep1((i+3)%4+6,rubic);
		opstep1(4,rubic);
		opstep1((i+3)%4,rubic);
		opstep1(4,rubic);
		opstep1(4,rubic);
		opstep1((i+3)%4+6,rubic);
		opstep1(4,rubic);
	}

	tem2=getcorner(rubic);
	while(tem2.cor[0][0]!='b')
	{
		opstep1(4,rubic);
		tem2=getcorner(rubic);
	}
	don=1;
	for(int i=0;i<4;i++)
	{
		if(tem1.ed[i][0]!=col[i]) don=0;
	}
	if(don==1) return;

	tem1=getedge(rubic);
	for(int i=0;i<4;i++)
	{
		if(tem1.ed[i][0]==col[i]) 
		{
			if(tem1.ed[(i+1)%4][0]==col[(i+2)%4])
			{
				opstep1((i+1)%4+6,rubic);
				opstep1(10,rubic);
				opstep1((i+1)%4,rubic);
				opstep1(10,rubic);
				opstep1((i+1)%4+6,rubic);
				opstep1(10,rubic);
				opstep1(10,rubic);
				opstep1((i+1)%4,rubic);
				opstep1(10,rubic);
				opstep1((i+3)%4,rubic);
				opstep1(4,rubic);
				opstep1((i+3)%4+6,rubic);
				opstep1(4,rubic);
				opstep1((i+3)%4,rubic);
				opstep1(4,rubic);
				opstep1(4,rubic);
				opstep1((i+3)%4+6,rubic);
				opstep1(4,rubic);
			}
			else
			{
				opstep1((i+3)%4,rubic);
				opstep1(4,rubic);
				opstep1((i+3)%4+6,rubic);
				opstep1(4,rubic);
				opstep1((i+3)%4,rubic);
				opstep1(4,rubic);
				opstep1(4,rubic);
				opstep1((i+3)%4+6,rubic);
				opstep1(4,rubic);
				opstep1((i+1)%4+6,rubic);
				opstep1(10,rubic);
				opstep1((i+1)%4,rubic);
				opstep1(10,rubic);
				opstep1((i+1)%4+6,rubic);
				opstep1(10,rubic);
				opstep1(10,rubic);
				opstep1((i+1)%4,rubic);
				opstep1(10,rubic);
			}
			break;
		}
		
	}
	cout<<"CONGRATULATIONS!!!"<<endl;
}

int stepind(char rubic[6][10])
{
	int ans=0;
	char col[4]={'b','r','g','o'};
	edge inied=getedge(rubic);
	corner inicor=getcorner(rubic);
	if((inied.ed[8][1]=='w')&&(inied.ed[8][0]=='b')&&(inied.ed[9][1]=='w')&&(inied.ed[9][0]=='r')&&(inied.ed[10][1]=='w')&&(inied.ed[10][0]=='g')&&(inied.ed[11][1]=='w')&&(inied.ed[11][0]=='o'))
	{
		ans=1;
		bool flag2=1;
		for(int i=0;i<4;i++)
		{
			if ((inicor.cor[i+4][2]!='w')||(inicor.cor[i+4][0]!=col[i])||(inicor.cor[i+4][1]!=col[(i+1)%4]))
			{
				flag2=0;
				break;
			}
		}
		if(flag2==1)
		{
			ans=2;
			bool flag3=1;
			for(int i=0;i<4;i++)
			{
				if((inied.ed[i+4][0]!=col[i])||(inied.ed[i+4][1]!=col[(i+1)%4]))
				{
					flag3=0;
					break;
				}
			}
			if(flag3==1)
			{
				ans=3;
				bool flag4=1;
				for(int i=0;i<4;i++)
				{
					if(inied.ed[i][1]!='y')
					{
						flag4=0;
						break;
					}
				}
				if(flag4==1)
				{
					ans=4;
					bool flag5=1;
					for(int i=0;i<4;i++)
					{
						if(inicor.cor[i][2]!='y')
						{
							flag5=0;
							break;
						}
					}
					if(flag5==1)
					{
						ans=5;
						bool flag6=1;
						for(int i=0;i<4;i++)
						{
							if(inicor.cor[i][1]!=inicor.cor[(i+1)%4][0])
							{
								flag6=0;
								break;
							}
						}
						if(flag6==1)
						{
							ans=6;
							bool flag7=1;
							for(int i=0;i<4;i++)
							{
								cout<<inied.ed[i][0]<<" "<<col[i];
							}
							for(int i=0;i<4;i++)
							{
								if(inied.ed[i][0]!=col[i])
								{
									flag7=0;
									break;
								}
							}
							if(flag7==1) ans=7;
						}
					}
				}
			}
		}
	}
	cout<<ans<<endl;
	return ans;
}

void solve(char rubic[6][10])
{ 
	int ind=stepind(rubic);
	if(ind<1)step0(rubic);
	if(ind<1)step1(rubic);
	if(ind<2)step2(rubic);
	if(ind<3)step3(rubic);
	if(ind<4)step4(rubic);
	if(ind<5)step5(rubic);
	if(ind<6)step6(rubic);
	if(ind<7)step7(rubic);
}



/*void Op(opera t,Scalar surfc[6],char rubic[6][10],Point3d axis[6],Point3d testp[8])
{
	char RUB[6][10];
	for(int i=0;i<6;i++)
	{
		for(int j=0;j<10;j++)
		{
		RUB[i][j]=rubic[i][j];
		}
	}

	if ((t.opnum==0)||(t.opnum==2)||(t.opnum==5))
	{
		char tem[10];
		for(int i=0;i<10;i++) tem[i]=rubic[t.opnum][i];
		if(t.opcode==0)
		{
		rubic[t.opnum][8]=tem[6];
		rubic[t.opnum][5]=tem[7];
		rubic[t.opnum][2]=tem[8];
		rubic[t.opnum][1]=tem[5];
		rubic[t.opnum][0]=tem[2];
		rubic[t.opnum][3]=tem[1];
		rubic[t.opnum][6]=tem[0];
		rubic[t.opnum][7]=tem[3];
		}
		else
		{
		rubic[t.opnum][8]=tem[2];
		rubic[t.opnum][7]=tem[5];
		rubic[t.opnum][6]=tem[8];
		rubic[t.opnum][3]=tem[7];
		rubic[t.opnum][0]=tem[6];
		rubic[t.opnum][1]=tem[3];
		rubic[t.opnum][2]=tem[0];
		rubic[t.opnum][5]=tem[1];
		}
	}
	else
	{
		char tem[10];
		for(int i=0;i<10;i++) tem[i]=rubic[t.opnum][i];
		if(t.opcode==0)
		{
	    rubic[t.opnum][8]=tem[2];
		rubic[t.opnum][7]=tem[5];
		rubic[t.opnum][6]=tem[8];
		rubic[t.opnum][3]=tem[7];
		rubic[t.opnum][0]=tem[6];
		rubic[t.opnum][1]=tem[3];
		rubic[t.opnum][2]=tem[0];
		rubic[t.opnum][5]=tem[1];
		}
		else
		{
	    rubic[t.opnum][8]=tem[6];
		rubic[t.opnum][5]=tem[7];
		rubic[t.opnum][2]=tem[8];
		rubic[t.opnum][1]=tem[5];
		rubic[t.opnum][0]=tem[2];
		rubic[t.opnum][3]=tem[1];
		rubic[t.opnum][6]=tem[0];
		rubic[t.opnum][7]=tem[3];
		}
	}
	switch (t.opnum)
	{
	case 0:
		{
			if(t.opcode==0)
			{
				for(int i=0;i<3;i++)
				{
				rubic[2][i]=RUB[5][i];
				rubic[5][i]=RUB
				}
			}

		}
	case 1:
		{
			if(t.opcode==1)
		}
	case 2:
		{
		}
	case 3:
		{
		}
	case 4:
		{
		}
	case 5:
		{
		}
	}

	/*double axanglet1=atan(axis[t.opnum].z/axis[t.opnum].y);
	double axanglet2=asin(axis[t.opnum].x);
	Point3d opLayer[8];
	for(int i=0;i<4;i++)
	{
		opLayer[i].x=testp[i].x;
		opLayer[i].y=testp[i].y;
		opLayer[i].z=testp[i].z;
	}
	
	Point3d stLayer[8];
	for(int i=4;i<8;i++)
	{
		stLayer[i].x=testp[i].x;
		stLayer[i].y=testp[i].y;
		stLayer[i].z=testp[i].z;
	}
	for(int i=4;i<8;i++)
	{
		opLayer[i].x=testp[i-4].x*2/3+testp[i].x*1/3;
		opLayer[i].y=testp[i-4].y*2/3+testp[i].y*1/3;
		opLayer[i].z=testp[i-4].z*2/3+testp[i].z*1/3;
		stLayer[i-4].x=opLayer[i].x;
		stLayer[i-4].y=opLayer[i].y;
		stLayer[i-4].z=opLayer[i].z;
	}
	for(int i=0;i<8;i++)
	{
	opLayer[i]=gtpoint(axanglet1,axanglet2,acos(-1)/100,opLayer[i].x,opLayer[i].y,opLayer[i].z);
	}
	double judge1=opLayer[0].x+opLayer[2].x;
	double judeg2=stLayer[0].x+opLayer[2].x;


}*/




void turnmat(int tem,char rubic[6][10])
{switch(tem)
{
	case '1':U(rubic);break;
	case '2':D(rubic);break;
	case '3':F(rubic);break;
	case '4':B(rubic);break;
	case '5':L(rubic);break;
	case '6':R(rubic);break;
	case 'q':Ui(rubic);break;
	case 'w':Di(rubic);break;
	case 'e':Fi(rubic);break;
	case 'r':Bi(rubic);break;
	case 't':Li(rubic);break;
	case 'y':Ri(rubic);break;
	case 'a':step0(rubic);break;
	case 's':step1(rubic);break;
	case 'd':step2(rubic);break;
	case 'f':step3(rubic);break;
	case 'g':step4(rubic);break;
	case 'h':step5(rubic);break;
	case 'j':step6(rubic);break;
	case 'k':step7(rubic);break;
	case 'b':
	{
			string op[12]={"U","D","F","B","L","R","Ui","Di","Fi","Bi","Li","Ri"};
			char RUB[6][10];
			int i;
			for(i=0;i<6;i++)
			{
				for(int j=0;j<10;j++)
				{
					RUB[i][j]=rubic[i][j];
				}
			}
			countstep=0;
			solve(RUB);
			num=0;
			if(countstep<=3)
			{
				for(int i=0;i<countstep;i++)
				{
					actualsteps[i]=presteps[i];
				}
				num=countstep;
				cout<<"Solving Algorithm:"<<endl;
				for(i=0;i<num;i++)
				{
					cout<<actualsteps[i]<<" ";
				}
				break;
			}
			string window[4];
			int flag=2;
			for(i=0;i<countstep-3;i++)
			{
				for(int j=0;j<4;j++)
				{
				window[j]=presteps[i+j];
				}
				if((window[0]==window[1])&&(window[1]==window[2])&&(window[2]==window[3])) 
				{
					i=i+3;
					continue;
					flag=0;
				}
				if((window[0]==window[1])&&(window[1]==window[2]))
				{
					flag=1;
					for(int p=0;p<12;p++)
					{
						if(op[p]==window[0]) 
						{
							actualsteps[num]=op[(p+6)%12];
							i=i+2;
							num++;

						}
					}
					if(i==countstep-2) 
					{
						actualsteps[num]=presteps[countstep-1];
						num++;
						break;
					}
				}

				else
				{
					actualsteps[num]=window[0];
					num++;
					if(i==countstep-4)
					{
						for(int j=1;j<4;j++)
						{
							actualsteps[num]=presteps[i+j];
							num++;
						}
						break;
					}
				}

			}

			while(flag!=2)
			{
				for(i=0;i<num;i++)
				{
					presteps[i]=actualsteps[i];
				}
				countstep=num;
				num=0;
				flag=2;
				for(i=0;i<countstep-3;i++)
				{
					for(int j=0;j<4;j++)
					{
					window[j]=presteps[i+j];
					}
					if((window[0]==window[1])&&(window[1]==window[2])&&(window[2]==window[3])) 
					{
						i=i+3;
						continue;
						flag=1;
					}
					if((window[0]==window[1])&&(window[1]==window[2]))
					{
						flag=0;
						for(int p=0;p<12;p++)
						{
							if(op[p]==window[0]) 
							{
								actualsteps[num]=op[(p+6)%12];
								i=i+2;
								num++;
							}
						}
						if(i>=countstep-2) 
						{
							actualsteps[num]=presteps[countstep-1];
							num++;
							break;
						}
					}

					else
					{
						actualsteps[num]=window[0];
						num++;
						if(i>=countstep-4)
						{
							for(int j=1;j<4;j++)
							{
								actualsteps[num]=presteps[i+j];
								num++;
							}
							break;
						}
					}
				}
			}
			if((actualsteps[num-1]==actualsteps[num-2])&&(actualsteps[num-2]==actualsteps[num-3]))
			{
			for(int p=0;p<12;p++)
			{
				if(op[p]==actualsteps[num-3]) 
				{
					actualsteps[num-3]=op[(p+6)%12];
					i=i+2;
					break;
				}
			}
			num=num-2;
			}
			cout<<"Solving Algorithm:"<<endl;
			for(i=0;i<num;i++)
			{
				cout<<actualsteps[i]<<" ";
			}
			
			
			break;

	}
	default:break;
	}
}

int main()
{
    Scalar surfc[26];
	surfc[24]=Scalar(0,255,255);
	surfc[22]=Scalar(250,250,250);
	surfc[1]=Scalar(255,0,0);
	surfc[6]=Scalar(0,255,0);
	surfc[14]=Scalar(10,128,250);
	surfc[17]=Scalar(0,0,255);
	char rubic[6][10]={{"yyyyyyyyy"},{"wwwwwwwww"},{"bbbbbbbbb"},{"ggggggggg"},{"ooooooooo"},{"rrrrrrrrr"}};
	int surface[6][4]={{0,1,2,3},{4,5,6,7},{0,3,7,4},{1,2,6,5},{2,3,7,6},{1,0,4,5}};
	int imsize=500;
	double angle1=acos(-1)/4;
	double angle2=0.3;
	double xs=0;
	double ys=0;
	int flag=0;
	int flagn=0;
	int sign=1;
	Point3d testp[8];
	testp[0]=Point3d(1,1,1);
	testp[1]=Point3d(-1,1,1);
	testp[2]=Point3d(-1,-1,1);
	testp[3]=Point3d(1,-1,1);
	testp[4]=Point3d(1,1,-1);
	testp[5]=Point3d(-1,1,-1);
	testp[6]=Point3d(-1,-1,-1);
	testp[7]=Point3d(1,-1,-1);
	Point3d axpoint[6];
	axpoint[0]=Point3d(1,0,0);
	axpoint[1]=Point3d(-1,0,0);
	axpoint[2]=Point3d(0,1,0);
	axpoint[3]=Point3d(0,-1,0);
	axpoint[4]=Point3d(0,0,1);
	axpoint[5]=Point3d(0,0,-1);

	Vec3b wcolor;
	wcolor[0]=255;
	wcolor[1]=255;
	wcolor[2]=255;
	Mat screen(imsize,imsize,CV_8UC3,Scalar(255,255,255));
	Mat words=imread("rubic.jpg");
	Mat imageROI=screen(Rect(250-words.cols/2,0,words.cols,words.rows));
	Mat mask= imread("rubic.jpg",0);  
	words.copyTo(imageROI,mask);
	for(int i=0;i<8;i++)
	{
		testp[i]=gtpoint(angle1,0,angle2,testp[i].x,testp[i].y,testp[i].z);
	}
	DrawCube(screen,imsize,testp,surface,surfc,rubic);
	int ifstep=0;
	bool iniflag=0;
	bool solveflag=0;
	while(1)
	{

	 if(_kbhit())
	 {
		char tem=_getch();
		cout<<tem<<endl;
	   turnmat(tem,rubic);
	   if(tem=='b')
		   {
			   iniflag=1;
			   solveflag=0;
	       }
	   else
	   {
		   solveflag=0;
	   }
	 }
	 if(iniflag==1)
	 {
		 ifstep=0;
		 iniflag=0;
		 solveflag=1;
	 }
	 if(solveflag==1)
	 {
	 if(ifstep%2==0)
		{
		if((ifstep/2)<num)	optest(actualsteps[ifstep/2],rubic);
		}
	 }
	if(ifstep<5000) ifstep++;
	 WCHAR Winname[]=L"Rubic's_Cube";
     HWND pwnd= FindWindow(NULL,Winname);
	 POINT curpos;
	 GetCursorPos(&curpos);
     ScreenToClient(pwnd,&curpos);
    if(KEY_DOWN(MOUSE_MOVED))
	{
		int xx,yy;
		if((curpos.x<imsize)&&(curpos.x>=0))xx=curpos.x;
		else if(curpos.x>=imsize)xx=imsize-1;
		else xx=0;
		if((curpos.y<imsize)&&(curpos.y>=0))yy=curpos.y;
		else if(curpos.y>=imsize)yy=imsize-1;
		else yy=0;
		Vec3b pixel=screen.at<Vec3b>(yy,xx);
		if(flag==0)
		{/*if(pixel[1]=)*/
			if((pixel[0]==255)&&(pixel[1]==255)&&(pixel[2]==255))
			{
				xs=curpos.x;
				ys=curpos.y;
				flagn=1;
			}
			flag=1;
		}
		else
		{
			if(flagn==1)
			{
				double x=curpos.x;
				double y=curpos.y;
				double dx=x-xs;
				double dy=ys-y;
				if((dx!=0)||(dy!=0))
				{
				if (dy==0)
				angle1=acos(-1)/2;
				else
				angle1=atan(-dx/dy);
				if (dy>0) sign=1;
				else if(dy<0) sign=-1;
				else
				{
					if(dx>0) sign=-1;
					else sign=1;
				}
				angle2=sign*0.01*sqrt(dx*dx+dy*dy);
				for(int i=0;i<8;i++)
				testp[i]=gtpoint(angle1,0,angle2,testp[i].x,testp[i].y,testp[i].z);
				}
				xs=x;
				ys=y;
	        }
		}
	}
	else	
	{
		flag=0;
		flagn=0;
	}
	Mat blank(imsize,imsize,CV_8UC3,Scalar(255,255,255));
	Mat imageROI=blank(Rect(250-words.cols/2,0,words.cols,words.rows));
	Mat mask= imread("rubic.jpg",0);  
	words.copyTo(imageROI,mask);
	screen=blank;

    DrawCube(screen,imsize,testp,surface,surfc,rubic);
	waitKey(50);

	}
}
	
