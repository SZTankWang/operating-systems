#ifndef TAXPAYER_H
#define TAXPAYER_H

class Taxpayer{
	private:
		int RID;
		char * firstName;
		char * lastName;
		int numOfDepd;
		double income;
		int  posCode;

	public:
		Taxpayer();
		~Taxpayer();
		int getRid();
		void setRid(int arg);
		char * getFirstName();
		void setFirstName(char * arg);
		char * getLastName();
		void setLastName(char * arg);
		int getNumOfDepd();
		void setNumOfDepd(int arg);
		double getIncome();
		void setIncome(double arg);
		int getPosCode();
		void setPosCode(int arg);
};

#endif