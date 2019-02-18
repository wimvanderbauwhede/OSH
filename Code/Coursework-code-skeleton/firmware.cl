__kernel void firmware(                     
   __global int* input1,                
   __global int* input2,                
   __global int* output,              
   __global int* status,              
   const int w1,           
   const int w2,           
   const unsigned int count)           
{
   int i = get_global_id(0); 
if (i==0) {
  *status=0;
}
   if(i < count) {
       output[i] = w1*input1[i] * input1[i] + w2*input2[i] * input2[i];
   }
} 
