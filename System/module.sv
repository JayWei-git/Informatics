// ------------------------------ 算术逻辑单元 --------------------------------
module alu #(
    parameter WIDTH = 32
) (
    input  logic [WIDTH-1:0] a, b,
    input  logic [3:0]       alu_control,
    output logic [WIDTH-1:0] result,
    output logic             zero
);
    always_comb begin
        case (alu_control)
            4'b0000: result = a & b;                             // AND
            4'b0001: result = a | b;                             // OR
            4'b0010: result = a + b;                             // 加法器
            4'b0110: result = a - b;                             // 减法器（通常由加法器实现）
            4'b0111: result = ($signed(a) < $signed(b)) ? 32'd1 : 32'd0; // 有符号 SLT
            4'b1100: result = ~(a | b);                          // NOR
            default: result = '0;                                // 非法控制码的安全值
        endcase
    end
    assign zero = (result == '0); // beq 借此判断 a-b 是否为零
endmodule

// ALU 的二级译码：主控制器给出“运算类别”，R 型指令再由 funct 精确选择。
module alu_control (
    input  logic [1:0] alu_op,
    input  logic [5:0] funct,
    output logic [3:0] alu_control
);
    always_comb begin
        case (alu_op)
            2'b00: alu_control = 4'b0010; // lw / sw / addi：地址或结果相加
            2'b01: alu_control = 4'b0110; // beq：相减后检测 zero
            2'b10: begin
                case (funct)
                    6'b100000: alu_control = 4'b0010; // add
                    6'b100010: alu_control = 4'b0110; // sub
                    6'b100100: alu_control = 4'b0000; // and
                    6'b100101: alu_control = 4'b0001; // or
                    6'b101010: alu_control = 4'b0111; // slt
                    6'b100111: alu_control = 4'b1100; // nor
                    default:   alu_control = 4'b0000;
                endcase
            end
            default: alu_control = 4'b0000;
        endcase
    end
endmodule


// --------------------------- 存储器与寄存器堆 -------------------------------
module data_memory #(
    parameter WIDTH = 32,
    parameter DEPTH = 256
) (
    input logic clk, mem_write, mem_read,
    input logic [WIDTH-1:0] address, write_data,
    output logic [WIDTH-1:0] read_data
);
    localparam ADDR_BITS = $clog2(DEPTH);
    logic [WIDTH-1:0] mem [0:DEPTH-1];
    // MIPS 地址为字节地址，去掉最低 2 位后得到 32 位字索引。
    wire [ADDR_BITS-1:0] word_addr = address[ADDR_BITS+1:2];
    always_ff @(posedge clk) begin
        if (mem_write) mem[word_addr] <= write_data;
    end
    assign read_data = mem_read ? mem[word_addr] : '0; // 单周期所需的组合读
endmodule

module instruction_memory #(
    parameter WIDTH = 32,
    parameter DEPTH = 256
) (
    input logic [WIDTH-1:0] address,
    output logic [WIDTH-1:0] instruction
);
    localparam ADDR_BITS = $clog2(DEPTH);
    logic [WIDTH-1:0] mem [0:DEPTH-1];
    wire [ADDR_BITS-1:0] word_addr = address[ADDR_BITS+1:2];
    assign instruction = mem[word_addr]; // 组合 ROM 式取指
endmodule

module reg_file #(
    parameter WIDTH = 32,
    parameter DEPTH = 32
) (
    input logic clk, rst,
    input logic [4:0] write_register, read_register_1, read_register_2,
    input logic [WIDTH-1:0] write_data,
    input logic reg_write,
    output logic [WIDTH-1:0] read_data_1, read_data_2
);
    logic [WIDTH-1:0] regs [0:DEPTH-1];
    // 异步低有效复位；写端口仅在时钟上升沿生效，且 $zero 不可写。
    always_ff @(posedge clk or negedge rst) begin
        if (!rst) begin
            for (int i = 0; i < DEPTH; i++) regs[i] <= '0;
        end else if (reg_write && write_register != 5'd0) begin
            regs[write_register] <= write_data;
        end
    end
    // 两个独立的组合读端口；读 $zero 始终返回 0。
    assign read_data_1 = (read_register_1 == 5'd0) ? '0 : regs[read_register_1];
    assign read_data_2 = (read_register_2 == 5'd0) ? '0 : regs[read_register_2];
endmodule

// ------------------------------- 基础单元 -----------------------------------
// 2选1选择器
module mux_2x1 #(
    parameter width = 32
) (
    input logic [width-1:0] a, b,
    input logic sel,
    output logic [width-1:0] y
);
    always_comb begin
        if (sel) y = b;
        else     y = a;
    end
endmodule

// 程序计数器：保存下一条待取指令的字节地址。
module pc #(
    parameter WIDTH = 32
) (
    input logic clk, rst,
    input logic [WIDTH-1:0] pc_in,
    output logic [WIDTH-1:0] pc_out
);
    always_ff @(posedge clk or negedge rst) begin
        if (!rst) pc_out <= '0;
        else      pc_out <= pc_in;
    end
endmodule

// 顺序执行时，下一条 32 位指令地址为 PC + 4 字节。
module pc_adder #(
    parameter WIDTH = 32
) (
    input logic [WIDTH-1:0] pc,
    output logic [WIDTH-1:0] pc_plus_4
);
    assign pc_plus_4 = pc + 4;
endmodule

// 将 32 位数左移两位，用于按字对齐的分支偏移和跳转目标。
module shift_left2 #(
    parameter WIDTH = 32
) (
    input logic [WIDTH-1:0] in,
    output logic [WIDTH-1:0] out
);
    assign out = in << 2;
endmodule

// I 型指令的 16 位立即数扩展为 32 位有符号数。
module sign_extend (
    input logic [15:0] in,
    output logic [31:0] out
);
    assign out = {{16{in[15]}}, in};
endmodule
