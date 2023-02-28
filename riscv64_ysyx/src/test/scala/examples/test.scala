package examples

import org.scalatest.freespec.AnyFreeSpec
import chisel3._
import chiseltest._
import org.scalatest.freespec.AnyFreeSpec
import chisel3.experimental.BundleLiterals._

import scala.collection.mutable


//首先先测试输入输出均为2的时候的状态
class mul_test  extends AnyFreeSpec with ChiselScalatestTester {

   //huangxinzedashabi


    "DUt pass is hxz shabi" in {
        test(new mul_for_test(32)).withAnnotations(Seq(WriteVcdAnnotation)) { dut => 
            dut.clock.setTimeout(0)
            for (i <- 0 until 1000){
               
                var num1 = ((Math.random  ) * 10000).toInt + 1
                var num2 = ((Math.random  ) * 10000).toInt + 1
                // val num1 = 1
                // val num2 = 100
                // dut.io.signed.poke(0)
                // // println(dut.io.Output_valid.peek())
                // do{
                //     dut.io.data(0).poke(num1)
                //     dut.io.data(1).poke(num2)
                //     dut.io.input_valid.poke(1.U.asBool)
                //     dut.clock.step()
                // }while(dut.io.Output_valid.peekInt() != 1)
                // val data0 = dut.io.result(0).peekInt().toInt

                dut.io1.signed.poke(1)
                // println(dut.io.Output_valid.peek())
                do{
                    dut.io1.data(0).poke(num1)
                    dut.io1.data(1).poke(num2)
                    dut.io1.input_valid.poke(1.U.asBool)
                    dut.clock.step()
                }while(dut.io1.Output_valid.peekInt() != 1)
                val data0 = num1.toLong * num2.toLong
                println("true data")
                println(num1,num2)
                // println(0xffffffff.toInt)
                // dut.clock.step()
                val data1 = dut.io1.result(0).peekInt().toInt
                println("answer data")
                println(data0,data1)

                if(data0 != data1) {
                    // println((dut.io.result(0).peekInt().toInt,dut.io.result(1).peekInt().toInt))
                   
                    assert(false);
                }
                dut.clock.step()
            }
        }
    }
}
