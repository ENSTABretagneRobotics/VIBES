#include "treeview.h"

TreeView::TreeView(QWidget* parent) : QTreeView(parent)
{

}

TreeView::~TreeView()
{

}

void TreeView::keyPressEvent(QKeyEvent *event)
{
	//Override keyPressEvent
	QTreeView::keyPressEvent(event);
	if(event->key() == Qt::Key_Delete)
	{
		emit deleteFigureEvent();
	}
	else if(event->key() == Qt::Key_H)
	{
		emit hideFigureEvent();
	}
	else if(event->key() == Qt::Key_S)
	{
		emit showFigureEvent();
	}
	else if(event->key() == Qt::Key_P)
	{
		emit propertiesEvent();
	}
}

void TreeView::mouseDoubleClickEvent(QMouseEvent *event)
{	
	//Overide mouseDoubleClickEvent
	QTreeView::mouseDoubleClickEvent(event);
	if(event->button() == Qt::LeftButton)
	{
		emit showFigureEvent();
	}
}
