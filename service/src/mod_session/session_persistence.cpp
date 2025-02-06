#include "mod_session/session_persistence.h"
#include "utils/date_time.h"
#include "mod_session/session_dao.h"
#include "utils/uuid_generator.h"

namespace zinpass::mod_session {

    bool SessionPersistence::add(const mod_session::Session &session) {
        const mod_session::SessionDAO session_dao;
        if (const repo::DaoStatus ret = session_dao.add(session);
            repo::DaoStatus::Success != ret) {
            return false;
            }
        return true;
    }

    bool SessionPersistence::remove(const std::string& session_id){
        if (const mod_session::SessionDAO session_dao;
            repo::DaoStatus::Success != session_dao.remove(session_id)) {
            return false;
        }
        return true;
    }

    int SessionPersistence::remove_all(void){
        const mod_session::SessionDAO session_dao;
        return session_dao.remove_all();
    }

    bool SessionPersistence::save_session(const std::string& session_id){
        return false;
    }

    mod_session::Session SessionPersistence::load_session(const std::string& session_id){
        return {};
    }

    int SessionPersistence::get_active_session_count(){
        return -1;
    }

    std::vector<mod_session::Session> SessionPersistence::list_all_sessions(){
        std::vector<mod_session::Session> sessions;
        return sessions;
    }


}