		%quando abbiamo un'unica generazione di startup sample e vogliamo dividerle a meta tra incu e outcu

		%ControlCenterServer

		ELEM_TYPE ControlCenterServer_Type(const rate CCS_send_StartUpSampling_request_rate,  const weight CCS_CheckWaterInland_prob_request, 
			const rate CCS_send_wired_getExpectedParametersInland_request_rate, const rate CCS_send_wired_getExpectedParametersOutGoing_request_rate,
			const rate CCS_send_wired_StartUpSampling_response_rate, const rate CCS_send_sample_rate, const rate CCS_send_wired_CheckWaterInland_response_rate,
			const rate CCS_send_wired_CheckWaterOutGoing_response_rate)

		BEHAVIOR

			ServerCalled(void;void) = 
				choice{
					<receive_wired_StartUpSampling_request, _> . 
						choice{
							<sample_inLand, inf(1, 0.5)> . <send_StartUpSampling_in_request, exp(CCS_send_StartUpSampling_request_rate)> . WaitingStartUpSampling(),
							<sample_outGoing, inf(1, 0.5)> . <send_StartUpSampling_out_request, exp(CCS_send_StartUpSampling_request_rate)> . WaitingStartUpSampling()
						},

					<receive_sample, _> . <send_sample, exp(CCS_send_sample_rate)> . ServerCalled(),
					<receive_wired_CheckWater_request, _> . 
						choice{
							<inLand, inf(1, CCS_CheckWaterInland_prob_request)> . <send_wired_getExpectedParametersInland_request, exp(CCS_send_wired_getExpectedParametersInland_request_rate)> . WaitingForInParameters(),
							<outGoing, inf(1, 1 - CCS_CheckWaterInland_prob_request)> . <send_wired_getExpectedParametersOutGoing_request, exp(CCS_send_wired_getExpectedParametersOutGoing_request_rate)> . WaitingForOutParameters()

						}
			};

			WaitingStartUpSampling(void;void) =
				<receive_StartUpSampling_response, _> . <send_wired_StartUpSampling_response, exp(CCS_send_wired_StartUpSampling_response_rate)> . ServerCalled();

			WaitingForInParameters(void;void) = 
				<receive_wired_getExpectedParametersInland_response, _> . <send_wired_CheckWater_response, exp(CCS_send_wired_CheckWaterInland_response_rate)> . ServerCalled();

			WaitingForOutParameters(void;void) = 
				<receive_wired_getExpectedParametersOutGoing_response, _> . <send_wired_CheckWater_response, exp(CCS_send_wired_CheckWaterOutGoing_response_rate)> . ServerCalled()


		INPUT_INTERACTIONS

			UNI receive_wired_StartUpSampling_request;
				receive_wired_CheckWater_request;
				receive_wired_getExpectedParametersInland_response;
				receive_wired_getExpectedParametersOutGoing_response

			OR  receive_StartUpSampling_response;
				receive_sample

		OUTPUT_INTERACTIONS

			UNI send_wired_StartUpSampling_response;
				send_wired_CheckWater_response;
				send_wired_getExpectedParametersInland_request;
				send_wired_getExpectedParametersOutGoing_request;
				send_sample;
				send_StartUpSampling_in_request;
				send_StartUpSampling_out_request