/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class cn_nlp_Tagger */

#ifndef _Included_cn_nlp_Tagger
#define _Included_cn_nlp_Tagger
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     cn_nlp_Tagger
 * Method:    tagSentence
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_cn_nlp_Tagger_tagSentence
  (JNIEnv *, jobject, jstring);

/*
 * Class:     cn_nlp_Tagger
 * Method:    init
 * Signature: (Ljava/lang/String;Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_cn_nlp_Tagger_init
  (JNIEnv *, jobject, jstring, jstring);

/*
 * Class:     cn_nlp_Tagger
 * Method:    tagFile
 * Signature: (Ljava/lang/String;Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_cn_nlp_Tagger_tagFile
  (JNIEnv *, jobject, jstring, jstring);

#ifdef __cplusplus
}
#endif
#endif
