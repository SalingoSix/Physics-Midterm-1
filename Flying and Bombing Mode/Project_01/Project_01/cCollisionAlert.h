#ifndef _cCollisionAlert_HG_
#define _cCollisionAlert_HG_

class cCollisionAlert
{
public:
	static cCollisionAlert* getInstance();
	void makeAlert(int obj1, int obj2, int type1, int type2);
	~cCollisionAlert();

private:
	cCollisionAlert();

	static cCollisionAlert* m_pTheOnlyAlerter;

};


#endif