; Below is an .ll file that, when executed, should die due to executing 
; undefined behavior.  The error is in the "sub nsw" -- a subtraction that 
; is not permitted to overflow.  Run this sucker through lli and then see 
; if you can find the right modification to trap the problem.
; 
; John
;
; ModuleID = 'small.c'

target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@x0 = global i32 -1, align 4
@x1 = common global i32 0, align 4

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
entry:
   %0 = load i32* @x0, align 4, !tbaa !1
   %cmp = icmp sgt i32 %0, 0
   %sub = sub nsw i32 2147483647, %0
   %cmp1 = icmp slt i32 %sub, 1
   %or.cond = and i1 %cmp, %cmp1
   br i1 %or.cond, label %lor.end.thread, label %lor.end

lor.end.thread:                                   ; preds = %entry
   store i32 1, i32* @x1, align 4, !tbaa !1
   br label %if.then

lor.end:                                          ; preds = %entry
   %tobool = icmp ne i32 %0, -1
   %lor.ext = zext i1 %tobool to i32
   store i32 %lor.ext, i32* @x1, align 4, !tbaa !1
   br i1 %tobool, label %if.then, label %if.end

if.then:                                  ; preds = %lor.end.thread, %lor.end
   %putchar = tail call i32 @putchar(i32 120) #1
   br label %if.end

if.end:                                   ; preds = %if.then, %lor.end
   ret i32 0
}

; Function Attrs: nounwind
declare i32 @putchar(i32) #1

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" 
"no-frame-pointer-elim"="false" "no-infs-fp-math"="false" 
"no-nans-fp-math"="false" "stack-protector-buffer-size"="8" 
"unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.6.0 (217983)"}
!1 = metadata !{metadata !2, metadata !2, i64 0}
!2 = metadata !{metadata !"int", metadata !3, i64 0}
!3 = metadata !{metadata !"omnipotent char", metadata !4, i64 0}
!4 = metadata !{metadata !"Simple C/C++ TBAA"}

