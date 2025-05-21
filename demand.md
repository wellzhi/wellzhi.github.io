graph TD
    %% 主要节点定义
    Start([开始]) --> AddDemand[新增需求]
    AddDemand --> ExamineStatus{审核状态}
    
    %% 审核流程
    ExamineStatus -->|审核中 ExamineStatusEnum.EXAMINING| ReleaseApproval[需求发布审核]
    ReleaseApproval --> ApprovalResult{审核结果}
    ApprovalResult -->|拒绝 TaskStateEnum.REJECT| RejectStatus[审核拒绝\nExamineStatus=0]
    ApprovalResult -->|同意 TaskStateEnum.AGREE| AgreeStatus[审核通过\nExamineStatus=1\nAssignStatus=UNASSIGNED]
    
    %% 分配流程
    AgreeStatus --> AssignProcess[客服分配]
    AssignProcess --> AssignType{分配类型}
    
    %% 分配给公司
    AssignType -->|分配给公司 AssignTypeEnum.COMPANY| AssignCompany[分配给公司\nAssignStatus=ASSIGNED\nAssignType=COMPANY\nDockingStatus=UN_DOCKING]
    AssignCompany --> CompanyApproval[公司审批对接]
    CompanyApproval --> CompanyResult{公司决定}
    CompanyResult -->|拒绝| CompanyReject[公司拒绝对接\nAssignStatus=REJECTED]
    CompanyResult -->|同意| CompanyAgree[公司确认对接\nDockingStatus=DOCKING]
    
    %% 分配给技术经理人
    AssignType -->|分配给技术经理人 AssignTypeEnum.MANAGER| AssignManager[分配给技术经理人\nAssignStatus=ASSIGNED\nAssignType=MANAGER\nDockingStatus=UN_DOCKING]
    AssignManager --> ManagerApproval[技术经理人审批对接]
    ManagerApproval --> ManagerResult{技术经理人决定}
    ManagerResult -->|拒绝| ManagerReject[技术经理人拒绝对接\nAssignStatus=REJECTED\nDockingStatus=REJECT]
    ManagerResult -->|同意| ManagerAgree[技术经理人确认对接\nDockingStatus=DOCKING]
    
    %% 对接完成流程
    CompanyAgree --> DockingProcess[对接过程]
    ManagerAgree --> DockingProcess
    DockingProcess --> DockingResult{对接结果}
    DockingResult -->|完成对接| DockingComplete[对接完成\nDockingStatus=COMPLETE]
    DockingResult -->|对接失败| DockingFail[对接失败\nDockingStatus=FAIL]
    
    %% 更新需求
    UpdateDemand[更新需求] --> ExamineStatus
    
    %% 参与人员标注
    classDef user fill:#d4f1f9,stroke:#333
    classDef manager fill:#ffcccc,stroke:#333
    classDef company fill:#e8f8f5,stroke:#333
    classDef system fill:#fdebd0,stroke:#333
    
    class AddDemand,UpdateDemand user
    class ReleaseApproval,AssignProcess system
    class AssignManager,CompanyApproval,ManagerApproval manager
    class CompanyAgree,CompanyReject,ManagerAgree,ManagerReject company
    
    %% 状态说明
    subgraph 审核状态-ExamineStatusEnum
        EXAMINING[审核中=2]
        AGREE[同意=1]
        REJECT[拒绝=0]
    end
    
    subgraph 分配状态-AssignStatusEnum
        UNASSIGNED[未分配=0]
        ASSIGNED[已分配=1]
        REJECTED[已拒绝=2]
    end
    
    subgraph 对接状态-DockingStatusEnum
        UN_DOCKING[未对接=0]
        DOCKING[对接中=1]
        COMPLETE[已完成=2]
        FAIL[失败=3]
        REJECT_DOCKING[拒绝=4]
    end
    
    subgraph 参与人员
        USER[用户]
        ROOT_MANAGER[客服]
        TECHNICAL_MANAGER[技术经理人]
        COMPANY[公司]
    end