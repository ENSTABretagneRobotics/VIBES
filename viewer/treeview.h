#ifndef HEADER_TREEVIEW
#define HEADER_TREEVIEW

#include <QTreeView>
#include <QKeyEvent>


class TreeView : public QTreeView
{
	//Same as a QTreeView but with overridden event handlers
	Q_OBJECT

		public:
			explicit TreeView(QWidget* parent);
			~TreeView();	

		protected:
			void keyPressEvent(QKeyEvent *event);
			void mouseDoubleClickEvent(QMouseEvent *event);

		signals:
    		void deleteFigureEvent();
    		void hideFigureEvent();
    		void showFigureEvent();
    		void propertiesEvent();

};			

#endif