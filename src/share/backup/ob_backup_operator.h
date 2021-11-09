/**
 * Copyright (c) 2021 OceanBase
 * OceanBase CE is licensed under Mulan PubL v2.
 * You can use this software according to the terms and conditions of the Mulan PubL v2.
 * You may obtain a copy of Mulan PubL v2 at:
 *          http://license.coscl.org.cn/MulanPubL-2.0
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PubL v2 for more details.
 */

#ifndef OCEANBASE_SHARE_BACKUP_OB_BACKUP_OPERATOR_H_
#define OCEANBASE_SHARE_BACKUP_OB_BACKUP_OPERATOR_H_

#include "lib/net/ob_addr.h"
#include "lib/mysqlclient/ob_mysql_proxy.h"
#include "lib/hash/ob_hashmap.h"
#include "share/ob_dml_sql_splicer.h"
#include "share/schema/ob_table_schema.h"
#include "lib/mysqlclient/ob_mysql_transaction.h"
#include "lib/mysqlclient/ob_mysql_result.h"
#include "ob_backup_struct.h"
#include "ob_backup_manager.h"

namespace oceanbase {
namespace share {

class ObITenantBackupTaskOperator {
public:
  ObITenantBackupTaskOperator() = default;
  virtual ~ObITenantBackupTaskOperator() = default;
  static int get_tenant_backup_task(const uint64_t tenant_id, const common::ObSqlString& sql,
      common::ObIArray<ObTenantBackupTaskItem>& items, common::ObISQLClient& sql_proxy);
  static int get_tenant_backup_history_task(const uint64_t tenant_id, const common::ObSqlString& sql,
      common::ObIArray<ObTenantBackupTaskItem>& items, common::ObISQLClient& sql_proxy);
  static int fill_task_item(const ObTenantBackupTaskItem& item, ObDMLSqlSplicer& dml);
  static int fill_task_history_item(
      const ObTenantBackupTaskItem& item, const bool need_fill_mark_deleted_item, ObDMLSqlSplicer& dml);
  static int fill_task_clean_history(const int64_t copy_id, const ObTenantBackupTaskItem& item, ObDMLSqlSplicer& dml);
  static int extract_tenant_backup_task(sqlclient::ObMySQLResult* result, ObTenantBackupTaskItem& item);
  static int get_tenant_ids(const uint64_t tenant_id, const common::ObSqlString& sql,
      common::ObIArray<uint64_t>& tenant_ids, common::ObISQLClient& sql_proxy);

private:
  static int fill_one_item(const ObTenantBackupTaskItem& item, ObDMLSqlSplicer& dml);
};

class ObTenantBackupTaskOperator {
public:
  ObTenantBackupTaskOperator() = default;
  virtual ~ObTenantBackupTaskOperator() = default;
  static int get_tenant_backup_task(const uint64_t tenant_id, const int64_t backup_set_id, const int64_t incarnation,
      ObTenantBackupTaskItem& item, common::ObISQLClient& proxy);
  static int insert_task(const ObTenantBackupTaskItem& item, common::ObISQLClient& proxy);
  static int report_task(const ObTenantBackupTaskItem& item, common::ObISQLClient& proxy);
  static int remove_task(const uint64_t tenant_id, const int64_t incarnation, const int64_t backup_set_id,
      common::ObISQLClient& sql_proxy);
  static int get_tenant_backup_task(
      const uint64_t tenant_id, const bool for_update, ObTenantBackupTaskItem& item, common::ObISQLClient& proxy);

private:
  static int remove_one_item(const uint64_t tenant_id, const int64_t incarnation, const int64_t backup_set_id,
      common::ObISQLClient& sql_proxy);
};

class ObPGBackupTaskOperator {
public:
  ObPGBackupTaskOperator() = default;
  virtual ~ObPGBackupTaskOperator() = default;
  static int get_pg_backup_task(ObPGBackupTaskItem& item, common::ObISQLClient& sql_proxy);
  static int get_pg_backup_task(common::ObIArray<ObPGBackupTaskItem>& items, common::ObMySQLProxy& sql_proxy);
  static int get_pg_backup_task(const uint64_t tenant_id, const int64_t incarnation, const int64_t backup_set_id,
      common::ObIArray<ObPGBackupTaskItem>& items, common::ObISQLClient& sql_proxy);
  static int batch_report_task(const common::ObIArray<ObPGBackupTaskItem>& items, common::ObISQLClient& proxy);
  static int batch_remove_task(const uint64_t tenant_id, const int64_t incarnation, const int64_t backup_set_id,
      const int64_t max_delete_rows, common::ObISQLClient& proxy, int64_t& affected_rows);
  static int get_finished_backup_task(const uint64_t tenant_id, const int64_t incarnation, const int64_t backup_set_id,
      common::ObIArray<ObPGBackupTaskItem>& items, common::ObISQLClient& sql_proxy);
  static int get_latest_backup_task(
      common::ObISQLClient& sql_proxy, const uint64_t tenant_id, ObPGBackupTaskItem& item);
  static int get_total_pg_task_count(common::ObISQLClient& sql_proxy, const uint64_t tenant_id, int64_t& task_count);
  static int update_pg_task_info(common::ObISQLClient& sql_proxy, const common::ObAddr& addr,
      const common::ObReplicaType& replica_type, const ObTaskId& trace_id,
      const ObPGBackupTaskInfo::BackupStatus& status, const common::ObPartitionKey& pkey);
  static int update_pg_backup_task_status(common::ObISQLClient& sql_proxy,
      const ObPGBackupTaskInfo::BackupStatus& status, const common::ObPartitionKey& pkey);
  static int update_result_and_status(common::ObISQLClient& sql_proxy, const ObPGBackupTaskInfo::BackupStatus& status,
      const int32_t result, const common::ObPartitionKey& pkey);
  static int get_one_doing_pg_task(const uint64_t tenant_id, const int64_t incarnation, const int64_t backup_set_id,
      common::ObISQLClient& sql_proxy, common::ObIArray<ObPGBackupTaskInfo>& pg_task_infos);
  static int get_pg_backup_task(const uint64_t tenant_id, const int64_t incarnation, const int64_t backup_set_id,
      const int64_t backup_task_id, common::ObIArray<ObPGBackupTaskItem>& items, common::ObISQLClient& sql_proxy);
  static int get_one_pg_task(const uint64_t tenant_id, const int64_t incarnation, const int64_t backup_set_id,
      common::ObISQLClient& sql_proxy, ObPGBackupTaskInfo& pg_task_info);
  static int get_pending_pg_task(const uint64_t tenant_id, const int64_t incarnation, const int64_t backup_set_id,
      common::ObISQLClient& sql_proxy, common::ObIArray<ObPGBackupTaskInfo>& pg_task_infos);
  static int update_result_and_status_and_statics(
      common::ObISQLClient& sql_proxy, const ObPGBackupTaskInfo& pg_task_info, int64_t& affected_rows);
  static int cancel_pending_tasks(const uint64_t tenant_id, const int64_t incarnation, const int64_t backup_set_id,
      const int64_t limit_num, common::ObISQLClient& sql_proxy);

private:
  static int fill_one_item(const ObPGBackupTaskItem& item, ObDMLSqlSplicer& dml);
  static int get_pg_backup_task(const uint64_t tenant_id, const common::ObSqlString& sql,
      common::ObIArray<ObPGBackupTaskItem>& items, common::ObISQLClient& sql_proxy);
  static int remove_one_item(const common::ObPGKey& pg_key, common::ObMySQLProxy& sql_proxy);
  static int extract_pg_task(common::sqlclient::ObMySQLResult* result, ObPGBackupTaskItem& item);
};

class ObTenantBackupInfoOperation {
public:
  static int update_info_item(common::ObISQLClient& sql_client, const uint64_t tenant_id, const ObBackupInfoItem& item);
  static int get_tenant_list(common::ObISQLClient& sql_client, common::ObIArray<uint64_t>& tenant_id_list);
  static int load_base_backup_info(common::ObISQLClient& sql_client, ObBaseBackupInfo& info);
  static int insert_base_backup_info(common::ObISQLClient& sql_client, ObBaseBackupInfo& info);
  static int remove_base_backup_info(common::ObISQLClient& sql_client, const uint64_t tenant_id);
  static int load_info_item(
      common::ObISQLClient& sql_client, const uint64_t tenant_id, ObBackupInfoItem& item, const bool need_lock = true);
  static int get_backup_snapshot_version(
      common::ObISQLClient& sql_proxy, const uint64_t tenant_id, int64_t& backup_snapshot_version);
  static int get_backup_schema_version(
      common::ObISQLClient& sql_proxy, const uint64_t tenant_id, int64_t& backup_schema_version);
  static int get_tenant_name_backup_schema_version(common::ObISQLClient& sql_proxy, int64_t& backup_schema_version);
  static int update_tenant_name_backup_schema_version(
      common::ObISQLClient& sql_proxy, const int64_t backup_schema_version);
  static int insert_info_item(common::ObISQLClient& sql_client, const uint64_t tenant_id, const ObBackupInfoItem& item);
  static int clean_backup_scheduler_leader(
      common::ObISQLClient& sql_client, const uint64_t tenant_id, const common::ObAddr& scheduler_leader);
  static int remove_info_item(common::ObISQLClient& sql_client, const uint64_t tenant_id, const char* name);

private:
  template <typename T>
  static int set_info_item(const char* name, const char* info_str, T& info);
  template <typename T>
  static int load_info(common::ObISQLClient& sql_client, T& info);
  template <typename T>
  static int insert_info(common::ObISQLClient& sql_client, T& info);
  static int get_backup_info_item_count(int64_t& cnt);
};

class ObBackupTaskHistoryOperator {
public:
  ObBackupTaskHistoryOperator() = default;
  virtual ~ObBackupTaskHistoryOperator() = default;
  static int get_tenant_backup_task(const uint64_t tenant_id, const int64_t backup_set_id, const int64_t incarnation,
      common::ObISQLClient& proxy, ObTenantBackupTaskItem& item);
  static int insert_task(const ObTenantBackupTaskItem& item, common::ObISQLClient& proxy);
  static int remove_task(const uint64_t tenant_id, const int64_t incarnation, const int64_t backup_set_id,
      common::ObISQLClient& sql_proxy);
  static int get_tenant_backup_tasks(
      const uint64_t tenant_id, common::ObISQLClient& proxy, common::ObIArray<ObTenantBackupTaskItem>& items);
  static int get_delete_backup_set_tasks(const uint64_t tenant_id, const int64_t backup_set_id,
      common::ObISQLClient& proxy, common::ObIArray<ObTenantBackupTaskItem>& items);
  static int get_expired_backup_tasks(const uint64_t tenant_id, const int64_t expired_time, common::ObISQLClient& proxy,
      common::ObIArray<ObTenantBackupTaskItem>& items);
  static int get_need_mark_deleted_backup_tasks(const uint64_t tenant_id, const int64_t incarnation,
      const int64_t backup_set_id, const ObBackupDest& backup_dest, const bool for_update, common::ObISQLClient& proxy,
      common::ObIArray<ObTenantBackupTaskItem>& items);
  static int mark_backup_task_deleted(
      const uint64_t tenant_id, const int64_t incarnation, const int64_t backup_set_id, common::ObISQLClient& proxy);
  static int delete_marked_task(const uint64_t tenant_id, common::ObISQLClient& proxy);
  static int get_mark_deleted_backup_tasks(
      const uint64_t tenant_id, common::ObISQLClient& proxy, common::ObIArray<ObTenantBackupTaskItem>& items);
  static int get_all_tenant_backup_set_matches(const int64_t backup_set_id, const bool is_smaller,
      common::ObISQLClient& sql_proxy, common::ObIArray<uint64_t>& tenant_list);

  static int get_task_in_time_range(const int64_t start_time, const int64_t end_time, common::ObISQLClient& proxy,
      common::ObIArray<ObTenantBackupTaskItem>& items);
  static int get_tenant_full_backup_tasks(
      const uint64_t tenant_id, common::ObISQLClient& proxy, common::ObIArray<ObTenantBackupTaskItem>& items);
  static int get_tenant_backup_task(const uint64_t tenant_id, const int64_t backup_set_id, const bool for_update,
      common::ObISQLClient& proxy, ObTenantBackupTaskItem& item);
  static int get_tenant_max_succeed_backup_task(
      const uint64_t tenant_id, common::ObISQLClient& proxy, ObTenantBackupTaskItem& item);
  static int get_all_tenant_backup_tasks(common::ObISQLClient& proxy, common::ObIArray<ObTenantBackupTaskItem>& items);
  static int get_tenant_ids_with_backup_set_id(
      const int64_t backup_set_id, common::ObISQLClient& proxy, common::ObIArray<uint64_t>& tenant_ids);
  static int get_tenant_ids_with_snapshot_version(
      const int64_t snapshot_version, common::ObISQLClient& proxy, common::ObIArray<uint64_t>& tenant_ids);
  static int update_backup_task_info(
      const ObTenantBackupTaskInfo& backup_task_info, const bool fill_mark_delete_item, common::ObISQLClient& proxy);

private:
  static int remove_one_item(const uint64_t tenant_id, const int64_t incarnation, const int64_t backup_set_id,
      common::ObISQLClient& sql_proxy);
};

class ObTenantBackupCleanInfoOperator {
public:
  ObTenantBackupCleanInfoOperator() = default;
  virtual ~ObTenantBackupCleanInfoOperator() = default;
  static int insert_clean_info(
      const uint64_t tenant_id, const ObBackupCleanInfo& clean_info, common::ObISQLClient& proxy);
  static int update_clean_info(const uint64_t tenant_id, const ObBackupCleanInfo& item, common::ObISQLClient& proxy);
  static int remove_clean_info(
      const uint64_t tenant_id, const ObBackupCleanInfo& clean_info, common::ObISQLClient& sql_proxy);
  static int get_tenant_clean_info(
      const uint64_t tenant_id, const bool for_update, ObBackupCleanInfo& clean_info, common::ObISQLClient& proxy);
  static int get_clean_info_status(
      const uint64_t tenant_id, common::ObISQLClient& proxy, ObBackupCleanInfoStatus::STATUS& status);
  static int get_deleted_tenant_clean_infos(
      common::ObISQLClient& sql_proxy, common::ObIArray<ObBackupCleanInfo>& deleted_tenant_clean_infos);
  static int extract_backup_clean_info(sqlclient::ObMySQLResult* result, ObBackupCleanInfo& clean_info);

private:
  static int fill_one_item(const ObBackupCleanInfo& clean_info, ObDMLSqlSplicer& dml);
  static int get_tenant_clean_info(const uint64_t tenant_id, const common::ObSqlString& sql,
      common::ObIArray<ObBackupCleanInfo>& clean_infos, common::ObISQLClient& sql_proxy);
  static int remove_one_item(
      const uint64_t tenant_id, const ObBackupCleanInfo& clean_info, common::ObISQLClient& sql_proxy);
};

class ObBackupCleanInfoHistoryOperator {
public:
  ObBackupCleanInfoHistoryOperator() = default;
  virtual ~ObBackupCleanInfoHistoryOperator() = default;
  static int insert_clean_info(const ObBackupCleanInfo& clean_info, common::ObISQLClient& proxy);
  static int remove_tenant_clean_info(const uint64_t tenant_id, common::ObISQLClient& sql_proxy);

private:
  static int fill_one_item(const ObBackupCleanInfo& clean_info, ObDMLSqlSplicer& dml);
  static int remove_one_item(const uint64_t tenant_id, common::ObISQLClient& sql_proxy);
};

class ObBackupTaskCleanHistoryOpertor {
public:
  ObBackupTaskCleanHistoryOpertor() = default;
  virtual ~ObBackupTaskCleanHistoryOpertor() = default;
  static int insert_task_info(const int64_t job_id, const int64_t copy_id,
      const ObTenantBackupTaskInfo& tenant_backup_task, common::ObISQLClient& proxy);
  static int remove_task_info(const uint64_t tenant_id, const int64_t job_id, common::ObISQLClient& sql_proxy);

private:
  static int fill_one_item(const int64_t job_id, const int64_t copy_id,
      const ObTenantBackupTaskInfo& tenant_backup_task, ObDMLSqlSplicer& dml);
  static int remove_one_item(const uint64_t tenant_id, const int64_t job_id, common::ObISQLClient& sql_proxy);
};

class ObBackupSetFilesOperator {
public:
  ObBackupSetFilesOperator() = default;
  virtual ~ObBackupSetFilesOperator() = default;
  static int get_tenant_backup_set_file_info(const uint64_t tenant_id, const int64_t backup_set_id,
      const int64_t incarnation, const int64_t copy_id, const bool for_update, common::ObISQLClient& proxy,
      ObBackupSetFileInfo& backup_set_file_info);
  static int get_backup_set_info_with_file_status(const uint64_t tenant_id, const int64_t incarnation,
      const bool for_update, const ObBackupFileStatus::STATUS& file_status, common::ObISQLClient& proxy,
      common::ObIArray<ObBackupSetFileInfo>& backup_set_file_info_array);
  static int insert_tenant_backup_set_file_info(
      const ObBackupSetFileInfo& backup_set_file_info, common::ObISQLClient& proxy);
  static int update_tenant_backup_set_file(const ObBackupSetFileInfo& src_backup_set_file_info,
      const ObBackupSetFileInfo& dest_backup_set_file_info, common::ObISQLClient& proxy);
  static int check_can_update_backup_set_file_info(const ObBackupSetFileInfo::BackupSetStatus& src_status,
      const ObBackupSetFileInfo::BackupSetStatus& dest_status, const ObBackupFileStatus::STATUS& src_file_status,
      const ObBackupFileStatus::STATUS& dest_file_status);
  static int get_tenant_backup_set_file_infos(const uint64_t tenant_id, const int64_t incarnation,
      const int64_t full_backup_set_id, const int64_t copy_id, const bool for_update, common::ObISQLClient& proxy,
      common::ObIArray<ObBackupSetFileInfo>& backup_set_file_info_array);
  static int get_tenant_backup_set_file_info(const uint64_t tenant_id, const int64_t backup_set_id,
      const int64_t copy_id, const bool for_update, common::ObISQLClient& proxy,
      ObBackupSetFileInfo& backup_set_file_info);
  static int get_backup_set_file_info_copies(const uint64_t tenant_id, const int64_t incarnation,
      const int64_t backup_set_id, common::ObISQLClient& proxy,
      common::ObIArray<ObBackupSetFileInfo>& backup_set_file_infos);
  static int get_backup_set_file_info_copy_list(const uint64_t tenant_id, const int64_t incarnation,
      const int64_t backup_set_id, const ObBackupBackupCopyIdLevel copy_id_level, common::ObISQLClient& proxy,
      common::ObIArray<ObBackupSetFileInfo>& backup_set_file_infos);
  static int get_all_cluster_level_backup_set_copy_count(const uint64_t tenant_id, const int64_t incarnation,
      const int64_t backup_set_id, common::ObISQLClient& proxy, int64_t& copy_count);

private:
  static int fill_one_item_(const ObBackupSetFileInfo& backup_set_file_info, ObDMLSqlSplicer& dml);
  static int extract_backup_set_info_(sqlclient::ObMySQLResult* result, ObBackupSetFileInfo& item);
  static int get_backup_set_file_infos_(
      const common::ObSqlString& sql, common::ObIArray<ObBackupSetFileInfo>& items, common::ObISQLClient& sql_proxy);
};

class ObBackupInfoOperator final {
public:
  static int get_inner_table_version(common::ObISQLClient& sql_proxy, ObBackupInnerTableVersion& version);
  static int set_inner_table_version(common::ObISQLClient& sql_proxy, const ObBackupInnerTableVersion& version);
  static int get_max_piece_id(common::ObISQLClient& sql_proxy, const bool for_update, int64_t& max_piece_id);
  static int set_max_piece_id(common::ObISQLClient& sql_proxy, const int64_t max_piece_id);
  static int get_max_piece_create_date(
      common::ObISQLClient& sql_proxy, const bool for_update, int64_t& max_piece_create_date);
  static int set_max_piece_create_date(common::ObISQLClient& sql_proxy, const int64_t max_piece_create_date);
  static int get_tenant_name_backup_schema_version(common::ObISQLClient& sql_proxy, int64_t& backup_schema_version);
  static int set_tenant_name_backup_schema_version(
      common::ObISQLClient& sql_proxy, const int64_t backup_schema_version);
  static int get_backup_leader(common::ObISQLClient& sql_proxy, common::ObAddr& leader, bool& has_leader);
  static int set_backup_leader(common::ObISQLClient& sql_proxy, const common::ObAddr& leader);
  static int clean_backup_leader(common::ObISQLClient& sql_proxy, const common::ObAddr& leader);

private:
  static int get_int_value_(common::ObISQLClient& sql_proxy, const bool for_update, const char* name, int64_t& value);
  static int set_int_value_(common::ObISQLClient& sql_proxy, const char* name, const int64_t& value);
  static int get_string_value_(
      common::ObISQLClient& sql_proxy, const bool for_update, const char* name, char* buf, int64_t buf_len);
  static int set_string_value_(common::ObISQLClient& sql_proxy, const char* name, const char* value);
};

}  // end namespace share
}  // end namespace oceanbase

#endif  // OCEANBASE_SHARE_BACKUP_OB_BACKUP_OPERATOR_H_
