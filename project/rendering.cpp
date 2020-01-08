// Framebuffer
glBindFramebuffer(GL_FRAMEBUFFER, fbo);
shibaCheckGlError();
glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, firstPassTextureId, 0);
shibaCheckGlError();

{% if development %}
static bool checked = false;
if (!checked)
{
	checked = true;
	auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	shibaCheckGlError();
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		shibaError() << "Framebuffer is incomplete!";
		return;
	}
	else
	{
		shibaLog() << "Framebuffer is complete.";
	}
}
{% endif %}

glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
shibaCheckGlError();
glDepthMask(GL_TRUE);
shibaCheckGlError();
glEnable(GL_DEPTH_TEST);
shibaCheckGlError();

// Pass 0 ribbons
glUseProgram(shibaProgramIds[ribbonsShader]);
shibaCheckGlError();
shibaSetFloatUniformsForShader(ribbonsShader);
shibaCheckGlError();
glDisable(GL_CULL_FACE);
shibaCheckGlError();
glBindVertexArray(vao);
shibaCheckGlError();
glDrawElements(GL_TRIANGLE_STRIP, indiceCount, GL_UNSIGNED_INT, indices);
shibaCheckGlError();

// Pass 1 particles
glUseProgram(shibaProgramIds[particlesShader]);
shibaCheckGlError();
shibaSetFloatUniformsForShader(particlesShader);
shibaCheckGlError();
glEnable(GL_CULL_FACE);
shibaCheckGlError();
glBindVertexArray(vaoParticles);
shibaCheckGlError();
glDrawElements(GL_TRIANGLE_STRIP, indiceParticleCount, GL_UNSIGNED_INT, indicesParticles);
shibaCheckGlError();

// Pass 2 post processing
glBindFramebuffer(GL_FRAMEBUFFER, shibaFinalFramebufferId);
shibaCheckGlError();
glUseProgram(shibaProgramIds[postFxShader]);
shibaCheckGlError();
shibaSetFloatUniformsForShader(postFxShader);
shibaCheckGlError();
//glDisable(GL_CULL_FACE);
//shibaCheckGlError();
glDisable(GL_DEPTH_TEST);
shibaCheckGlError();
{% if development %}
	glActiveTexture(GL_TEXTURE0 + 0);
	shibaCheckGlError();
	glBindTexture(GL_TEXTURE_2D, firstPassTextureId);
	shibaCheckGlError();
	shibaSetSampler2DUniformsForShader(postFxShader);
	shibaCheckGlError();
{% endif %}
//glClear(GL_COLOR_BUFFER_BIT);
//shibaCheckGlError();
shibaDrawScreenRect();
shibaCheckGlError();
