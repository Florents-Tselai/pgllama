// #include "postgres.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "llama.h"


int main(int argc, char ** argv) {
    // path to the model gguf file
    char *model_path = "/tmp/tinyllama-1.1b-1t-openorca.Q2_K.gguf";
    // prompt to generate text from
    char  *prompt = "This is a story about a wolf learning SQL";
    // number of layers to offload to the GPU
    int ngl = 99;
    // number of tokens to predict
    int n_predict = 32;

    // parse command line arguments


    // initialize the model

    struct llama_model_params model_params = llama_model_default_params();
    model_params.n_gpu_layers = ngl;

    struct  llama_model * model = llama_load_model_from_file(model_path, model_params);

    if (model == NULL) {
        fprintf(stderr , "%s: error: unable to load model\n" , __func__);
        return 1;
    }

    // tokenize the prompt

    // find the number of tokens in the prompt
    const int n_prompt = -llama_tokenize(model, prompt, strlen(prompt), NULL, 0, true, true);

    // allocate space for the tokens and tokenize the prompt
    llama_token *prompt_tokens = malloc(n_prompt * sizeof(llama_token));
    if (llama_tokenize(model, prompt, strlen(prompt), prompt_tokens, n_prompt, true, true) < 0) {
        fprintf(stderr, "%s: error: failed to tokenize the prompt\n", __func__);
        return 1;
    }

    // initialize the context

    struct llama_context_params ctx_params = llama_context_default_params();
    // n_ctx is the context size
    ctx_params.n_ctx = n_prompt + n_predict - 1;
    // n_batch is the maximum number of tokens that can be processed in a single call to llama_decode
    ctx_params.n_batch = n_prompt;
    // enable performance counters
    ctx_params.no_perf = false;

    struct llama_context * ctx = llama_new_context_with_model(model, ctx_params);

    if (ctx == NULL) {
        fprintf(stderr , "%s: error: failed to create the llama_context\n" , __func__);
        return 1;
    }

    // initialize the sampler

   struct llama_sampler_chain_params sparams = llama_sampler_chain_default_params();
    sparams.no_perf = false;
    struct llama_sampler * smpl = llama_sampler_chain_init(sparams);

    llama_sampler_chain_add(smpl, llama_sampler_init_greedy());

    // print the prompt token-by-token
    //
    // for (auto id : prompt_tokens) {
    //     char buf[128];
    //     int n = llama_token_to_piece(model, id, buf, sizeof(buf), 0, true);
    //     if (n < 0) {
    //         fprintf(stderr, "%s: error: failed to convert token to piece\n", __func__);
    //         return 1;
    //     }
    //     std::string s(buf, n);
    //     printf("%s", s.c_str());
    // }

    // prepare a batch for the prompt

    llama_batch batch = llama_batch_get_one(prompt_tokens, n_prompt);

    // main loop

    // const auto t_main_start = ggml_time_us();
    int n_decode = 0;
    llama_token new_token_id;

    for (int n_pos = 0; n_pos + batch.n_tokens < n_prompt + n_predict; ) {
        // evaluate the current batch with the transformer model
        if (llama_decode(ctx, batch)) {
            fprintf(stderr, "%s : failed to eval, return code %d\n", __func__, 1);
            return 1;
        }

        n_pos += batch.n_tokens;

        // sample the next token
        {
            new_token_id = llama_sampler_sample(smpl, ctx, -1);

            // is it an end of generation?
            if (llama_token_is_eog(model, new_token_id)) {
                break;
            }

            char buf[128];
            int n = llama_token_to_piece(model, new_token_id, buf, sizeof(buf), 0, true);
            if (n < 0) {
                fprintf(stderr, "%s: error: failed to convert token to piece\n", __func__);
                return 1;
            }
            printf("%.*s", n, buf);  // %.*s prints exactly 'n' characters from 'buf'
            fflush(stdout);

            // prepare the next batch with the sampled token
            batch = llama_batch_get_one(&new_token_id, 1);

            n_decode += 1;
        }
    }

    printf("\n");

    // const auto t_main_end = ggml_time_us();

    // fprintf(stderr, "%s: decoded %d tokens in %.2f s, speed: %.2f t/s\n",
    //         __func__, n_decode, (t_main_end - t_main_start) / 1000000.0f, n_decode / ((t_main_end - t_main_start) / 1000000.0f));

    fprintf(stderr, "\n");
    llama_perf_sampler_print(smpl);
    llama_perf_context_print(ctx);
    fprintf(stderr, "\n");

    llama_sampler_free(smpl);
    llama_free(ctx);
    llama_free_model(model);

    return 0;
}