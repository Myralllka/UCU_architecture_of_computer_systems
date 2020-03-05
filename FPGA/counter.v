module counter(clk, reset, out);

input clk, reset;
output reg[3:0] out = 0;

always@(posedge clk, negedge reset) begin
	if(~reset)
		out <= 0;
	else
		// 1'b1 -- 1 digit in decimal and value.
		out <= out + 1'b1;
end

endmodule
