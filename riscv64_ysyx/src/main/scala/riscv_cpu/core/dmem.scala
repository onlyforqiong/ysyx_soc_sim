package examples

import chisel3._
import chisel3.stage._
import chisel3.util._


class dmem extends Module with riscv_macros {//hi = Input(UInt(32.W))lo寄存器
        //完全没用到chisel真正好的地方，我是废物呜呜呜呜
    val io = IO(new Bundle {
    val    req     = Input(UInt(1.W))
    val    addr_ok = Input(UInt(1.W))
    // val    data_stage2_stall = Input(Bool())
    val    data_ok = Input(UInt(1.W))
    val    rdata   = Input(UInt(data_length.W))

    val    ReadEn      = Input(UInt(1.W)) 
    val    Physisc_Address       = Input(UInt(data_length.W)) //p Physisc_Address
    val    WIDTH   = Input(UInt(2.W)) 
    val    SIGN    = Input(UInt(1.W))
    
    val    RD      = Output(UInt(data_length.W))
    val    data_pending = Output(UInt(1.W))
    })
    // val RD_Reg = RegInit(0.U(32.W))

    //val pending_reg = RegInit(0.U(1.W))
    // val has_data_req = RegInit(0.U.asBool)

    // // pending_reg := data_pending_Reg
    // // io.data_pending := data_pending_Reg
    // // 对req加一个延迟
    // has_data_req := io.req.asBool//Mux(io.req.asBool,1.U,Mux(io.data_ok.asBool,0.U,has_data_req))
    // pending_reg := Mux(has_data_req && !io.data_ok.asBool,1.U,Mux())
    
    io.data_pending := !io.data_ok
    // data_pending_Reg := Mux(( io.req.asBool),1.U, //req应该仅仅保持一个周期
    //         Mux(io.data_ok.asBool,0.U,data_pending_Reg))
    val ra = io.Physisc_Address(1,0)
    val third_ra = io.Physisc_Address(2)
    val data_ok_reg = RegInit(0.U(1.W))
    data_ok_reg := io.data_ok
    val rdata_reg = RegInit(0.U(32.W))
    rdata_reg := Mux(io.data_ok.asBool,io.rdata,rdata_reg)

    def get_byte(data:UInt,offset:UInt,sign:UInt):UInt = MuxLookup(Cat(offset,sign),0.U,Seq(
        "b00_1".U -> sign_extend(data(7,0),8),
        "b00_0".U -> unsign_extend(data(7,0),8),
        "b01_1".U -> sign_extend(data(15,8),8),
        "b01_0".U -> unsign_extend(data(15,8),8),
        "b10_1".U -> sign_extend(data(23,16),8),
        "b10_0".U -> unsign_extend(data(23,16),8),
        "b11_1".U -> sign_extend(data(31,24),8),
        "b11_0".U -> unsign_extend(data(31,24),8)))

    def get_halfword(data:UInt,offset:UInt,sign:UInt):UInt = MuxLookup(Cat(offset,sign),0.U,Seq(
        "b00_1".U -> sign_extend(data(15,0),16),
        "b00_0".U -> unsign_extend(data(15,0),16),
        "b10_1".U -> sign_extend(data(31,16),16),
        "b10_0".U -> unsign_extend(data(31,16),16)))
    val true_data = io.rdata
    // def get_word(data)
    def get_word(data : UInt,sign:UInt) :UInt = {
      
        Mux(sign.asBool,sign_extend(data,32),unsign_extend(data,32))
    }
    // val word_data = io.rdata
        /* Mux(io.data_ok.asBool,io.rdata,rdata_reg)*/ //；为了满足cache命中，一周期读取完的要求，并且未命中可以接上cache的那边的时序

    val all_ok = (data_ok_reg.asBool && io.ReadEn.asBool)
    io.RD := MuxLookup(io.WIDTH,0.U,Seq(
        0.U -> Mux(third_ra,get_byte(true_data(data_length - 1,32),ra,io.SIGN),get_byte(true_data(31,0),ra,io.SIGN)),
        1.U -> Mux(third_ra,get_halfword(true_data(data_length - 1 ,32),ra,io.SIGN), get_halfword(true_data(31,0),ra,io.SIGN)),//有符号扩展或者无符号扩展
        2.U -> Mux(third_ra,get_word(true_data(data_length - 1,32),io.SIGN),get_word(true_data(31,0),io.SIGN)),
        3.U -> true_data ))
    
}
// object dmem_test extends App{
//     (new ChiselStage).emitVerilog(new dmem)
// }

