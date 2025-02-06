#include "widget_public_method.h"

void set_table_widget_column_non_editable(QTableWidget* table_widget, const int column) {
    const int rowCount = table_widget->rowCount();
    for (int row = 0; row < rowCount; ++row) {
        QTableWidgetItem* item = table_widget->item(row, column);
        if (item == nullptr) {
            item = new QTableWidgetItem();
            table_widget->setItem(row, column, item);
        }
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    }
}
