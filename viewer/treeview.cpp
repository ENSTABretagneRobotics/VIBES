#include "treeview.h"

TreeView::TreeView(QWidget *parent) : QTreeView(parent)
{
	contextMenuFigure = new QMenu;
    contextMenuGroup = new QMenu;
    contextMenuFigure->addAction("Hide figure");
    contextMenuFigure->addAction("Open figure");
    contextMenuFigure->addAction("Close figure");
    contextMenuGroup->addAction("Edit properties");
	this->setContextMenuPolicy(Qt::CustomContextMenu); //Enable menu
	connect(this,SIGNAL(customContextMenuRequested(const QPoint &)),this,SLOT(openMenu(const QPoint &)));
	connect(contextMenuFigure,SIGNAL(triggered(QAction *)),this,SLOT(contextMenuTrigger(QAction *)));
	connect(contextMenuGroup,SIGNAL(triggered(QAction *)),this,SLOT(contextMenuTrigger(QAction *)));
}

TreeView::~TreeView()
{
	delete contextMenuFigure;
	delete contextMenuGroup;
}

void TreeView::contextMenuTrigger(QAction *action)
{
	if(action->text() == "Hide figure")
	{
		emit hideFigureEvent();
	}
	else if(action->text() == "Open figure")
	{
		emit showFigureEvent();
	}
	else if(action->text() == "Close figure")
	{
		emit deleteFigureEvent();
	}
	else if(action->text() == "Edit properties")
	{
		emit propertiesEvent();
	}
}

void TreeView::openMenu(const QPoint &point)
{
	//Open menu on right click
 	QModelIndex index = this->indexAt(point);
    if(index.isValid())
    {
    	if(index.parent().isValid()) //Check if the selection has a parent and thus is a group
    	{
        	contextMenuGroup->exec(this->viewport()->mapToGlobal(point));
    	}
    	else
    	{
    		contextMenuFigure->exec(this->viewport()->mapToGlobal(point));
    	}   
    } 
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
	else if(event->key() == Qt::Key_O)
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
		emit propertiesEvent();
	}
}
