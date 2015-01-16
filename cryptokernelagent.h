// Author: Dmitry Kukovinets (d1021976@gmail.com)

// This file contains cryptokernel agent. Agent must use Kernel for all kinds
// of cryptography tasks: generate new ids, encode or decode data...
// Agent is interface for GUI to do all cryptography actions.

// Example: [     GUI      ] <-> [          Agent          ] <-> [    Kernel    ]
//          (Qt/ncurses/...)     (depends on GUI and Kernel)     (lib/daemon/...)
// This version of program uses Qt GUI, so Agent has Qt-based API too. Agent may has
// own cache for some data. Some data caches in PasswordItem or GroupItem objects.

#ifndef CRYPTOKERNELAGENT_H
#define CRYPTOKERNELAGENT_H

#include <QObject>
#include <QString>

#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "cryptokernel/cryptokernel.h"
#include "mainwindow.h"
#include "warningwindow.h"

#include "abstractitem.h"
#include "groupitem.h"
#include "recorditem.h"
#include "typeitem.h"


class CryptoKernelAgent
{
public:
	CryptoKernelAgent();
	~CryptoKernelAgent();
	
  // Other functions
	void run();
  // End of other functions
	
  // GUI management
	MainWindow * GUI_mainWindow();
	
	// Update functions
	void GUI_updateRecordListItems();
	void GUI_updateRecordContent();
	
	// Record content
	void GUI_onNameClicked();
	void GUI_onNameChanged(QString newName);
	
	void GUI_onTypeClicked();
	void GUI_onTypeChanged(QString newTypeName);
	
	void GUI_onGroupClicked();
	void GUI_onGroupChanged(QString newGroupName);
	
	void GUI_onFieldClicked(int index);
	void GUI_onFieldChanged(int index, QString newText);
	
	void GUI_onMainWindowClosed();
	
	// Add group/record/type, remove selected
	void GUI_addGroup();
	void GUI_addRecord();
	void GUI_addType();
	void GUI_removeSelectedItems();
	
	void GUI_showWarning(const QString &title, const QString &text);
	void GUI_closeWarning();
  // End of GUI management
private:
	QString DATA_typeName(RecordItem *recordItem) const;
	QString DATA_parentGroupName(RecordItem *recordItem) const;
	
	void DATA_showRecordInList(RecordItem *recordItem);
	void DATA_hideRecordInList(RecordItem *recordItem);
	
  // GUI management (see cryptokernelagent_gui.cpp)
	MainWindow *mainWindow_;		// Main window attached to this agent
	WarningWindow *warningWindow_;	// Window with warning text (new window)
  // End of GUI management
	
	
  // Other functions
	void DATA_removeRecord(RecordItem *item);
	void DATA_removeGroup(GroupItem *item);
	void DATA_removeType(TypeItem *item);
  // End of other functions
	
	
  // Data management (see cryptokernelagent_data.cpp)
	cryptokernel kernel_;			// Cryptokernel used by this agent
	
	
	struct Groups {	// Groups maps
		struct GroupInfo {
			group_id_t id;
			GroupItem *item;
			
			QString name;
		};
		typedef std::list<GroupInfo> Container;
		typedef Container::iterator iterator;
		
		
		Container list;
		std::unordered_map<group_id_t, iterator> idsMap;
		std::unordered_map<GroupItem *, iterator> itemsMap;
		// The root group in mainWindow->leftPanel->groupListWidget availible as
		// groupIdsMap[kernel_.root_group_id()]
		
		
		bool add(const GroupInfo &info);
		void erase(group_id_t id);
		void erase(GroupItem *item);
		
		inline bool cached(group_id_t id);
		inline bool cached(GroupItem *item);
		
		void clear();
	};	// struct Groups
	Groups groups_;
	
	void DATA_addRootGroup();	// Unsafe! Use loadData()!
	void DATA_removeRootGroup();	// Unsafe! Use removeSelectedItems() or removeGroup()!
	
	void DATA_loadGroups();	// Unsafe! Use loadData()!
	
	
	struct Records {	// Records maps and set of shown items
		struct RecordInfo {
			record_id_t id;
			RecordItem *item;
			QTreeWidgetItem *recordListItem;
			
			QString name;
		};
		typedef std::list<RecordInfo> Container;
		typedef Container::iterator iterator;
		
		
		Container list;
		std::unordered_map<record_id_t, iterator> idsMap;
		std::unordered_map<RecordItem *, iterator> itemsMap;
		std::unordered_map<QTreeWidgetItem *, iterator> recordListItemsMap;
		
		
		// Records are shown in record list in mainWindow->mainWidget->recordListWidget
		// This set is part of set of all "QTreeWidgetItem *"s im recordItemsMap
		std::unordered_set<QTreeWidgetItem *> shownRecordListItems;
		
		
		bool add(const RecordInfo &info);
		void erase(record_id_t id);
		void erase(RecordItem *item);
		void erase(QTreeWidgetItem *recordListItem);
		
		inline bool cached(record_id_t id);
		inline bool cached(RecordItem *item);
		
		void clear();
	};	// struct Records
	Records records_;
	
	bool DATA_loadRecord(record_id_t id);
	void DATA_loadRecords();	// Unsafe! Use loadData()!
	
	
	struct Types {	// Type maps and root group
		struct TypeInfo {
			type_id_t id;
			TypeItem *item;
			
			QString name;
		};
		typedef std::list<TypeInfo> Container;
		typedef Container::iterator iterator;
		
		
		Container list;
		std::unordered_map<type_id_t, iterator> idsMap;
		std::unordered_map<TypeItem *, iterator> itemsMap;
		GroupItem *rootGroup = nullptr;	// Root group of types in mainWindow->leftPanel->groupListWidget
		
		
		bool add(const TypeInfo &info);
		void erase(type_id_t id);
		void erase(TypeItem *item);
		
		inline bool cached(type_id_t id);
		inline bool cached(TypeItem *item);
		
		void clear();
	};	// struct Types
	Types types_;
	
	void DATA_loadTypes();	// Unsafe! Use loadData()!
	
	
	struct RecordContent {	// Shown record content
		record_id_t shownRecordId;
		std::vector<rfield_id_t> shownFieldIds;
		
		void clear();
	};	// struct RecordContent
	RecordContent recordContent_;
	
	
	void DATA_loadData();	// Safe
  // End of data management
	
	
	// Deprecated constructor and operator=()
	[[deprecated("Don't copy the agent!")]]
	CryptoKernelAgent(const CryptoKernelAgent &other);
	
	[[deprecated("Don't copy the agent!")]]
	CryptoKernelAgent & operator=(const CryptoKernelAgent &other);
};


#include "cryptokernelagent_data.hpp"
#endif // CRYPTOKERNELAGENT_H
