#ifndef TELECOM_OPERATOR_DAO_H
#define TELECOM_OPERATOR_DAO_H

#include <string>
#include <vector>
#include "common/dao_status.h"
#include "connection_pool.h"

namespace zinpass::repo{

class TelecomOperatorDAO {
public:
    explicit TelecomOperatorDAO();
    ~TelecomOperatorDAO();
    DaoStatus findList(std::vector<std::string>& telecom_operators) const;

private:
    ConnectionPool& pool_;
};

}

#endif

