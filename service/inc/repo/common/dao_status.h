#ifndef DAO_STATUS_H
#define DAO_STATUS_H

namespace zinpass::repo {

enum class DaoStatus {
    Success,            // 操作成功
    NotFound,           // 数据不存在
    DuplicateKey,       // 唯一键冲突
    DatabaseBusy,       // 数据库被锁定（如事务未提交）
    ReadOnlyError,      // 数据库只读（权限问题）
    ConfigurationError, // API使用错误（如未正确关闭stmt）
    InvalidData,        // 数据无效（如类型错误、格式错误如无效时间字符串）
    OptimisticLock,     // 乐观锁冲突
    GenericError        // 其他未知错误
    // 数据库底层错误用异常传递，其他状态码按需扩展...
};

}

#endif

