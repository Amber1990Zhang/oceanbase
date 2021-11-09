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

#ifndef OCEANBASE_SHARE_BACKUP_OB_TENANT_BACKUP_TASK_UPDATER_H_
#define OCEANBASE_SHARE_BACKUP_OB_TENANT_BACKUP_TASK_UPDATER_H_

#include "share/ob_define.h"
#include "ob_backup_struct.h"
#include "ob_backup_operator.h"

namespace oceanbase {
namespace share {

class ObTenantBackupTaskUpdater {
public:
  ObTenantBackupTaskUpdater();
  virtual ~ObTenantBackupTaskUpdater() = default;
  int init(common::ObISQLClient& sql_proxy);
  int insert_tenant_backup_task(const ObBaseBackupInfoStruct& info, const ObExternBackupInfo& extern_backup_info);
  int get_tenant_backup_task(const uint64_t tenant_id, const int64_t backup_set_id, const int64_t incarnation,
      ObTenantBackupTaskInfo& tenant_backup_task);
  int get_tenant_backup_task(const uint64_t tenant_id, ObTenantBackupTaskInfo& tenant_backup_task);
  int get_tenant_backup_task(
      const uint64_t tenant_id, const bool for_update, ObTenantBackupTaskInfo& tenant_backup_task);
  int get_tenant_backup_task(const uint64_t tenant_id, const bool for_update, common::ObISQLClient& trans,
      ObTenantBackupTaskInfo& tenant_backup_task);
  int update_tenant_backup_task(const ObTenantBackupTaskInfo& src_info, const ObTenantBackupTaskInfo& dest_info);
  int update_tenant_backup_task(
      common::ObISQLClient& trans, const ObTenantBackupTaskInfo& src_info, const ObTenantBackupTaskInfo& dest_info);
  int remove_task(const uint64_t tenant_id, const int64_t incarnation, const int64_t backup_set_id);
  int insert_tenant_backup_task(const ObTenantBackupTaskInfo& tenant_backup_task);

private:
  int check_can_update_backup_task(
      const ObTenantBackupTaskInfo::BackupStatus& src_status, const ObTenantBackupTaskInfo::BackupStatus& dest_status);

private:
  bool is_inited_;
  common::ObISQLClient* sql_proxy_;
  DISALLOW_COPY_AND_ASSIGN(ObTenantBackupTaskUpdater);
};

class ObBackupTaskHistoryUpdater {
public:
  ObBackupTaskHistoryUpdater();
  virtual ~ObBackupTaskHistoryUpdater() = default;
  int init(common::ObISQLClient& sql_proxy);
  int insert_tenant_backup_task(const ObTenantBackupTaskInfo& task_info);

  int get_tenant_backup_tasks(const uint64_t tenant_id, const bool is_backup_backup,
      common::ObIArray<ObTenantBackupTaskInfo>& tenant_backup_tasks);
  int remove_task(const uint64_t tenant_id, const int64_t incarnation, const int64_t backup_set_id);
  int get_need_mark_deleted_backup_tasks(const uint64_t tenant_id, const int64_t backup_set_id,
      const int64_t incarnation, const ObBackupDest& backup_dest, const bool for_update,
      common::ObIArray<ObTenantBackupTaskInfo>& tenant_backup_tasks);
  int mark_backup_task_deleted(const uint64_t tenant_id, const int64_t incarnation, const int64_t backup_set_id);
  int delete_marked_task(const uint64_t tenant_id);
  int get_mark_deleted_backup_tasks(
      const uint64_t tenant_id, common::ObIArray<ObTenantBackupTaskInfo>& tenant_backup_tasks);
  int delete_backup_task(const ObTenantBackupTaskInfo& tenant_backup_task);
  int get_all_tenant_backup_tasks_in_time_range(
      const int64_t start_time, const int64_t end_time, common::ObIArray<ObTenantBackupTaskInfo>& tenant_backup_tasks);
  int get_tenant_full_backup_tasks(
      const uint64_t tenant_id, common::ObIArray<ObTenantBackupTaskInfo>& tenant_backup_tasks);
  int get_tenant_backup_task(const uint64_t tenant_id, const int64_t backup_set_id, const int64_t copy_id,
      const bool for_update, common::ObIArray<ObTenantBackupTaskInfo>& tenant_backup_tasks);
  int get_tenant_max_succeed_backup_task(const uint64_t tenant_id, ObTenantBackupTaskInfo& tenant_backup_task);
  int get_all_tenant_backup_tasks(common::ObIArray<ObTenantBackupTaskInfo>& tenant_backup_tasks);
  int get_tenant_ids_with_backup_set_id(
      const int64_t backup_set_id, const int64_t copy_id, common::ObIArray<uint64_t>& tenant_ids);
  int get_tenant_ids_with_snapshot_version(
      const int64_t snapshot_version, const bool is_backup_backup, common::ObIArray<uint64_t>& tenant_ids);
  int get_backup_set_file_info(const uint64_t tenant_id, const int64_t backup_set_id, const int64_t copy_id,
      const bool for_update, ObBackupSetFileInfo& backup_set_file_info);
  int mark_backup_set_info_deleting(const int64_t tenant_id, const int64_t backup_set_id, const int64_t copy_id);
  int delete_backup_set_info(const int64_t tenant_id, const int64_t backupset_id_id, const int64_t copy_id);
  int get_backup_set_file_info_copies(const int64_t tenant_id, const int64_t incarnation, const int64_t backup_set_id,
      common::ObIArray<ObBackupSetFileInfo>& backup_set_file_infos);
  int get_original_tenant_backup_task(const uint64_t tenant_id, const int64_t backup_set_id, const bool for_update,
      ObTenantBackupTaskInfo& tenant_backup_task);

private:
  int update_backup_task_info(const ObTenantBackupTaskInfo& backup_task_info);
  int update_backup_set_file_info(
      const ObBackupSetFileInfo& src_backup_set_file_info, const ObBackupSetFileInfo& dst_backup_set_file_info);
  int mark_backup_task_info_deleted(const ObTenantBackupTaskInfo& backup_task_info);

private:
  bool is_inited_;
  common::ObISQLClient* sql_proxy_;
  DISALLOW_COPY_AND_ASSIGN(ObBackupTaskHistoryUpdater);
};

}  // namespace share
}  // namespace oceanbase

#endif /* OCEANBASE_SHARE_BACKUP_OB_TENANT_BACKUP_TASK_UPDATER_H_ */
