; ModuleID = 'lls/test.ll'
source_filename = "src/test.c"
target datalayout = "e-m:o-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "arm64-apple-macosx14.0.0"

@.str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

; Function Attrs: noinline nounwind optnone ssp uwtable(sync)
define i32 @Compute(i32 noundef %0, i32 noundef %1) #0 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  store i32 %0, ptr %3, align 4
  store i32 %1, ptr %4, align 4
  store i32 0, ptr %5, align 4
  store i32 0, ptr %6, align 4
  br label %7

7:                                                ; preds = %15, %2
  %8 = load i32, ptr %6, align 4
  call void @__my_log_i32(i32 11, i32 %8)
  %9 = load i32, ptr %3, align 4
  call void @__my_log_i32(i32 12, i32 %9)
  %10 = icmp slt i32 %8, %9
  br i1 %10, label %11, label %18

11:                                               ; preds = %7
  %12 = load i32, ptr %5, align 4
  call void @__my_log_i32(i32 15, i32 %12)
  %13 = load i32, ptr %4, align 4
  call void @__my_log_i32(i32 16, i32 %13)
  %14 = add nsw i32 %12, %13
  call void @__my_log_i32(i32 17, i32 %14)
  store i32 %14, ptr %5, align 4
  br label %15

15:                                               ; preds = %11
  %16 = load i32, ptr %6, align 4
  call void @__my_log_i32(i32 20, i32 %16)
  %17 = add nsw i32 %16, 1
  call void @__my_log_i32(i32 21, i32 %17)
  store i32 %17, ptr %6, align 4
  br label %7, !llvm.loop !6

18:                                               ; preds = %7
  %19 = load i32, ptr %5, align 4
  call void @__my_log_i32(i32 24, i32 %19)
  ret i32 %19
}

; Function Attrs: noinline nounwind optnone ssp uwtable(sync)
define i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  store i32 0, ptr %1, align 4
  %3 = call i32 @Compute(i32 noundef 4, i32 noundef 7)
  call void @__my_log_i32(i32 29, i32 %3)
  store i32 %3, ptr %2, align 4
  %4 = load i32, ptr %2, align 4
  call void @__my_log_i32(i32 31, i32 %4)
  %5 = call i32 (ptr, ...) @printf(ptr noundef @.str, i32 noundef %4)
  call void @__my_log_i32(i32 32, i32 %5)
  ret i32 0
}

declare i32 @printf(ptr noundef, ...) #1

declare void @__my_log_i32(i32, i32)

declare void @__my_log_i64(i32, i64)

attributes #0 = { noinline nounwind optnone ssp uwtable(sync) "frame-pointer"="non-leaf-no-reserve" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="apple-m1" "target-features"="+aes,+altnzcv,+ccdp,+ccidx,+ccpp,+complxnum,+crc,+dit,+dotprod,+flagm,+fp-armv8,+fp16fml,+fptoint,+fullfp16,+jsconv,+lse,+neon,+pauth,+perfmon,+predres,+ras,+rcpc,+rdm,+sb,+sha2,+sha3,+specrestrict,+ssbs,+v8.1a,+v8.2a,+v8.3a,+v8.4a,+v8a" }
attributes #1 = { "frame-pointer"="non-leaf-no-reserve" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="apple-m1" "target-features"="+aes,+altnzcv,+ccdp,+ccidx,+ccpp,+complxnum,+crc,+dit,+dotprod,+flagm,+fp-armv8,+fp16fml,+fptoint,+fullfp16,+jsconv,+lse,+neon,+pauth,+perfmon,+predres,+ras,+rcpc,+rdm,+sb,+sha2,+sha3,+specrestrict,+ssbs,+v8.1a,+v8.2a,+v8.3a,+v8.4a,+v8a" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 2, !"SDK Version", [2 x i32] [i32 14, i32 0]}
!1 = !{i32 1, !"wchar_size", i32 4}
!2 = !{i32 8, !"PIC Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 1}
!4 = !{i32 7, !"frame-pointer", i32 4}
!5 = !{!"Homebrew clang version 22.1.5"}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
