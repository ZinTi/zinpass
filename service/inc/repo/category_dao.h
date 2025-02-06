#ifndef CATEGORY_DAO_H
#define CATEGORY_DAO_H

#include <string>
#include <vector>
#include "connection_pool.h"
#include "common/dao_status.h"

namespace zinpass::repo{

class CategoryDAO {
public:
    explicit CategoryDAO();
    ~CategoryDAO();

    /**
     * @brief categoryNameToId 将 categoryName 转换成对应的 id 主键
     * @param categoryName 手机号
     * @param id 查询到的id主键，如果未查询到则 id 为 -1
     * @return 成功与否
     */
    DaoStatus categoryNameToId(const std::string& categoryName, short* id) const;

    /**
     * @brief 查询类别
     * @param categories 类别
     * @return 成功与否
     */
    DaoStatus findCategories(std::vector<std::string>& categories) const;

private:
    ConnectionPool& pool_;

};

}

#endif


