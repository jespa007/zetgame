/*
 * CSGNode2d.h
 *
 *  Created on: Oct 4, 2014
 *      Author: jespada
 */

#pragma once
class CSGNode2dAnimation;
class CTexture;
class CSGNode2d:public CSGNode{
protected:

	//void applyTransform2d();
	//CMaterial 	*m_material,
	int m_selectedIndex,m_selectedIndexOld;
	//CQuadf *m_quad3d;
	CPoint2i position_aux;
	Sint16 dim_w,dim_h;
	//bool fixed_dimensions;
	CSGNode2dAnimation  *animation;
	CSGNode2dChannels components;
public:

	void setSGNode2dAnimation(CSGNode2dAnimation *mp);


	CSGNode2d();

	void setPositionX(int x);
	void setPositionY(int y);
	void setPosition(int x, int y);
	void setPosition(CPoint2i * pos);
	CPoint2i * getPosition();

	void setRotate(double angle);
	//void setRotate(double x, double y, double z);
	void setScale(float mul);
	void setScale(float mul_w, float mul_h);

	virtual void setDimensions(Sint16 w, Sint16 h);

	virtual void setWidth(Sint16 w);
	void setHeight(Sint16 h);

	virtual Sint16 getWidth();
	virtual Sint16 getHeight();

	/**
	 * The same as setSize but doesn't change dimensions on change image.
	 */
	//void setFixedDimensions(bool _h);
	//virtual void update();

	//virtual void draw();

	//virtual void draw()=0;

	virtual ~CSGNode2d();
};




