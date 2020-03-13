`define COUNTER_WIDTH 6 // macros

module counter (clk, reset, direction, set, set_value, out);

parameter WIDTH = `COUNTER_WIDTH; // config var, value by default

input clk, reset, direction, set;
input [WIDTH-1:0] set_value;
output reg [WIDTH-1:0] out = 0;

reg r_direction = 0; // one bit locale var, register

always @(posedge clk, posedge reset, posedge set)
begin
	if (reset)
		out <= 0;
	else if (set)
		out <= set_value;
	else if (r_direction)
		out <= out + 1'b1; // nonblocking assign width-format-value
	else if (~r_direction)
		out <= out - 1'b1;
end

always @(posedge direction)
	r_direction <= ~r_direction;

endmodule
