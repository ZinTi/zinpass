-- 密码管理系统日志部分
PRAGMA foreign_keys= ON;       -- 每次连接数据库后执行
BEGIN TRANSACTION;

-- 登录认证日志表 (记录所有认证相关事件)
CREATE TABLE auth_logs (
    id VARCHAR(36) PRIMARY KEY,          -- UUID主键
    timestamp DATETIME NOT NULL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW')),
    user_id VARCHAR(36) NOT NULL,        -- 用户UUID
    session_id VARCHAR(36) NOT NULL,     -- 会话UUID
    ip_address VARCHAR(45) NOT NULL,
    user_agent TEXT NOT NULL,
    action VARCHAR(50) NOT NULL CHECK(action IN (
        'login_attempt', 
        'login_success', 
        'login_failure', 
        'logout',
        'two_factor_triggered',
        'two_factor_failed'
    )),
    status VARCHAR(20) NOT NULL CHECK(status IN ('success', 'failure', 'pending')),
    username_attempted VARCHAR(255),     -- 登录尝试的用户名
    failure_reason TEXT,
    details JSON                          -- 额外信息(设备指纹等)
);

-- 密码操作日志表 (记录所有密码条目相关操作)
CREATE TABLE password_ops_logs (
    id VARCHAR(36) PRIMARY KEY,          -- UUID主键
    timestamp DATETIME NOT NULL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW')),
    user_id VARCHAR(36) NOT NULL,        -- 用户UUID
    session_id VARCHAR(36) NOT NULL,     -- 会话UUID
    action VARCHAR(50) NOT NULL CHECK(action IN (
        'password_create', 
        'password_read', 
        'password_update', 
        'password_delete',
        'password_export',
        'password_share_init',
        'password_share_accept'
    )),
    target_password_id VARCHAR(36) NOT NULL,  -- 操作的密码条目UUID
    encrypted_changes TEXT,               -- 加密的变更信息(AES加密)
    status VARCHAR(20) NOT NULL CHECK(status IN ('success', 'failure', 'partial')),
    device_fingerprint VARCHAR(64),       -- 设备指纹哈希
    details JSON
);

-- 系统事件日志表 (记录关键系统级事件)
CREATE TABLE system_events_logs (
    id VARCHAR(36) PRIMARY KEY,          -- UUID主键
    timestamp DATETIME NOT NULL DEFAULT (STRFTIME('%Y-%m-%d %H:%M:%f', 'NOW')),
    user_id VARCHAR(36),                 -- 可空(系统事件可能无用户)
    session_id VARCHAR(36),              -- 可空
    event_type VARCHAR(50) NOT NULL CHECK(event_type IN (
        'vault_lock', 
        'vault_unlock', 
        'key_rotation',
        'backup_created',
        'backup_restored',
        'security_settings_changed',
        'emergency_access_triggered'
    )),
    severity VARCHAR(10) NOT NULL CHECK(severity IN ('info', 'warning', 'critical')),
    description TEXT NOT NULL,
    parameters JSON                      -- 事件参数
);

-- 创建索引优化查询性能
CREATE INDEX idx_auth_logs_user ON auth_logs(user_id, timestamp);
CREATE INDEX idx_auth_logs_session ON auth_logs(session_id);
CREATE INDEX idx_pwd_ops_user ON password_ops_logs(user_id, timestamp);
CREATE INDEX idx_pwd_ops_target ON password_ops_logs(target_password_id);
CREATE INDEX idx_sys_events_time ON system_events_logs(timestamp);
CREATE INDEX idx_sys_events_type ON system_events_logs(event_type);

COMMIT;
