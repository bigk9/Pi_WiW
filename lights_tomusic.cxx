#include <iostream>
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <fstream>

#define high 29
#define mid 28
#define low 27
#define button 25
#define button2 24
#define button3 23

//to run on startup, change path in "sudo nano /etc/rc.local"

using namespace std;

void playMusic (string fileName);

int main()
{
	wiringPiSetup();
	pinMode(button, INPUT);
	pinMode(button2, INPUT);
	pinMode(button3, INPUT);

	playMusic("/home/pi/Documents/c++/lights_to_music/start");
	while (true)
	{
		digitalWrite (high, 0);
		digitalWrite (mid, 0);
		digitalWrite (low, 0);
		
		if (digitalRead(button)==1)
		{

			playMusic("/home/pi/Documents/c++/lights_to_music/WizinWin");
		}
		if (digitalRead(button2)==1)
		{

			playMusic("/home/pi/Documents/c++/lights_to_music/CoTB.txt");
		}
		if (digitalRead(button3)==1)
		{

			playMusic("/home/pi/Documents/c++/lights_to_music/jb.txt");
		}
	}
	return 0;	
}

void playMusic (string fileName)
{
	
	//setup Pi
	wiringPiSetup();
	pinMode(high, OUTPUT);
	pinMode(mid, OUTPUT);
	pinMode(low, OUTPUT);
	
	/*
	 * load file from main
	 * first line is song name
	 * second line is path to music
	 * next is bpm
	 * next is offset delay
	 * then number of quarter notes per measure
	 * then the light sequence
	 */
	  
	ifstream inF;
	inF.open(fileName.c_str());
	
	string name;
	getline(inF, name);
	
	cout << "Now Playing: " << name << endl;
	cout << "----------------------------------------------"<<endl;
	string musicPath;
	getline(inF, musicPath);

	
	double bpm;
	inF>>bpm;
	double tmp=60000/bpm;
	
	cout << "BPM: " << bpm << endl<<endl<<endl;
	
	int del;
	inF>>del;
	
	//set up for displaying measure
	int measure=1;
	double qtr, btCount=0;
	inF>>qtr;
	
	//plays music in background
	int pid;
	pid=fork();
	if(pid==0)
	{
			execlp("/usr/bin/omxplayer"," ", musicPath.c_str(), NULL);
			_exit(0);
	}
	else
	{					
		//figure out delay for syncing
		
		delay(del);
		
		/*
		 * each line consists of 4 numbers, 
		 * first number is quarter note multiplier
		 * next 3 numbers are the lights for that note (2 is on, 1 is off)
		 * a 0 ends the song
		 */
		 
		 cout << endl<<endl;
		 double mult; 
		 int h,m,l;
		 inF>>mult>>h>>m>>l;
		 //cout<<measure<<endl;
		 //measure++; 

		 
		 while (mult)
		 {
			 /*display current measure
			 btCount+=mult;
			 if(btCount==qtr)
			 {
				 cout<< measure<< endl;
				 measure++;
				 btCount = 0;
			 }
			 */
			 
			 digitalWrite(high, h-1);
			 digitalWrite(mid, m-1);
			 digitalWrite(low, l-1);
			 delay(tmp*mult);

			 
			 inF>>mult>>h>>m>>l;
		 }
	}
	
	// end music and lights
	system ("killall omxplayer.bin");
	digitalWrite(high, 0);
	digitalWrite(mid, 0);
	digitalWrite(low, 0);
	
	cout << endl << endl << endl;
	
	return;
}
